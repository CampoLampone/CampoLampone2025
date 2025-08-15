import json
import threading
import time
import atexit
from pathlib import Path
from typing import Dict, List
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.encoders import jsonable_encoder
from pydantic import BaseModel

# --------------------
# Config
# --------------------
PERSIST_FILE = Path("store/store.json")
SAVE_DELAY = 10       # Seconds after last change before persisting
MAX_INTERVAL = 60     # Max seconds between writes even if activity continues

# --------------------
# FastAPI setup
# --------------------
app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # change for production
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# --------------------
# Models
# --------------------
class Phase(BaseModel):
    color: str
    duration: int

class Light(BaseModel):
    phases: List[Phase] = []

class Outages(BaseModel):
    enabled: bool
    period: int
    duration: int

class Schedule(BaseModel):
    lights: List[Light] = []
    outages: Outages

# --------------------
# Store and persistence logic
# --------------------
store_lock = threading.Lock()
store: Dict[str, Schedule] = {}

_last_change_time = 0.0
_last_persist_time = 0.0
_save_timer = None

def _persist():
    """Write store to persistent JSON file."""
    global _last_persist_time
    with store_lock:
        with PERSIST_FILE.open("w") as f:
            json.dump(jsonable_encoder(store), f, indent=2)
    _last_persist_time = time.time()

def _maybe_flush():
    """Flush if delay exceeded or max interval exceeded."""
    global _save_timer
    now = time.time()
    time_since_change = now - _last_change_time
    time_since_persist = now - _last_persist_time

    if time_since_change >= SAVE_DELAY or time_since_persist >= MAX_INTERVAL:
        _persist()
    else:
        _save_timer = threading.Timer(
            min(SAVE_DELAY - time_since_change, MAX_INTERVAL - time_since_persist),
            _maybe_flush
        )
        _save_timer.daemon = True
        _save_timer.start()

def schedule_save():
    """Call after each change to schedule persistence."""
    global _last_change_time, _save_timer
    _last_change_time = time.time()
    if not _save_timer or not _save_timer.is_alive():
        _maybe_flush()

def load_store():
    """Load store from file at startup, create it if missing."""
    if not PERSIST_FILE.exists():
        # Make sure parent folders exist
        PERSIST_FILE.parent.mkdir(parents=True, exist_ok=True)
        # Create an empty store.json file
        with PERSIST_FILE.open("w") as f:
            json.dump({}, f, indent=2)
        return  # Nothing to load yet

    with PERSIST_FILE.open() as f:
        data = json.load(f)
    with store_lock:
        store.clear()
        for k, v in data.items():
            store[k] = Schedule(**v)
    global _last_persist_time
    _last_persist_time = time.time()

def shutdown_flush():
    """Ensure store is written on shutdown."""
    if _save_timer:
        _save_timer.cancel()
    _persist()

# Register shutdown handler
atexit.register(shutdown_flush)

# Load initial data
load_store()

# --------------------
# API endpoints
# --------------------
@app.get("/controllers", response_model=List[str])
def list_controllers():
    with store_lock:
        return list(store.keys())

@app.get("/controllers/{controller_id}", response_model=Schedule)
def get_schedule(controller_id: str):
    with store_lock:
        sched = store.get(controller_id)
        if not sched:
            raise HTTPException(status_code=404, detail="Schedule not found")
        return sched

@app.put("/controllers/{controller_id}")
def save_schedule(controller_id: str, schedule: Schedule):
    with store_lock:
        store[controller_id] = schedule
    schedule_save()
    return {"status": "ok"}

@app.delete("/controllers/{controller_id}")
def delete_controller(controller_id: str):
    with store_lock:
        if controller_id in store:
            del store[controller_id]
            schedule_save()
            return {"status": "deleted"}
    raise HTTPException(status_code=404, detail="Not found")
