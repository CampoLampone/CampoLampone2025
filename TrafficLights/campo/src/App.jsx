import React, { useState, useEffect } from "react";
import PhaseTable from "./PhaseTable";
import axios from "axios";

const API_PORT = 8000;
const API_BASE = `${window.location.protocol}//${window.location.hostname}:${API_PORT}`;

const DEFAULT_OUTAGES = { enabled: false, period: 120, duration: 6 };

function App() {
  const [controllers, setControllers] = useState([]);
  const [selectedController, setSelectedController] = useState("");
  const [schedule, setSchedule] = useState({
    lights: [],
    outages: DEFAULT_OUTAGES,
  });
  const [showModal, setShowModal] = useState(false);
  const [newControllerName, setNewControllerName] = useState("");
  const [dirty, setDirty] = useState(false);

  const loadControllers = async () => {
    try {
      const res = await axios.get(`${API_BASE}/controllers`);
      setControllers(res.data);
    } catch {
      alert("Failed to load controllers");
    }
  };

  const loadSchedule = async (id) => {
    try {
      const res = await axios.get(`${API_BASE}/controllers/${id}`);
      setSchedule({
        lights: res.data.lights || [],
        outages: res.data.outages || { ...DEFAULT_OUTAGES },
      });
      setDirty(false);
    } catch {
      setSchedule({ lights: [], outages: { ...DEFAULT_OUTAGES } });
      setDirty(false);
    }
  };

  const saveSchedule = async () => {
    try {
      await axios.put(
        `${API_BASE}/controllers/${selectedController}`,
        schedule,
      );
      alert("Saved!");
      loadControllers();
      setDirty(false);
    } catch {
      alert("Error saving schedule");
    }
  };

  const deleteController = async () => {
    if (!selectedController) return alert("Select a controller first");
    if (
      window.confirm(
        `Are you sure you want to delete controller "${selectedController}"?`,
      )
    ) {
      axios
        .delete(`${API_BASE}/controllers/${selectedController}`)
        .then(() => {
          loadControllers();
          setSelectedController("");
          setSchedule({ lights: [], outages: { ...DEFAULT_OUTAGES } });
          setDirty(false);
        })
        .catch(() => alert("Failed to delete controller"));
    }
  };

  const addLight = () => {
    setSchedule((s) => ({
      ...s,
      lights: [...s.lights, { phases: [] }],
    }));
    setDirty(true);
  };

  const deleteLight = (lightIdx) => {
    if (!window.confirm("Delete this light?")) return;
    setSchedule((s) => ({
      ...s,
      lights: s.lights.filter((_, i) => i !== lightIdx),
    }));
    setDirty(true);
  };

  const updateLightPhases = (lightIdx, newPhases) => {
    setSchedule((s) => {
      const newLights = [...s.lights];
      newLights[lightIdx] = { ...newLights[lightIdx], phases: newPhases };
      return { ...s, lights: newLights };
    });
    setDirty(true);
  };

  const addPhaseToLight = (lightIdx) => {
    setSchedule((s) => {
      const newLights = [...s.lights];
      newLights[lightIdx] = {
        ...newLights[lightIdx],
        phases: [...newLights[lightIdx].phases, { color: "red", duration: 10 }],
      };
      return { ...s, lights: newLights };
    });
    setDirty(true);
  };

  const deletePhase = (lightIdx, phaseIdx) => {
    setSchedule((s) => {
      const newLights = [...s.lights];
      newLights[lightIdx] = {
        ...newLights[lightIdx],
        phases: newLights[lightIdx].phases.filter((_, i) => i !== phaseIdx),
      };
      return { ...s, lights: newLights };
    });
    setDirty(true);
  };

  const createController = async () => {
    if (dirty && !window.confirm("You have unsaved changes. Continue?")) return;
    if (!newControllerName.trim()) return;
    await axios.put(`${API_BASE}/controllers/${newControllerName}`, {
      lights: [],
      outages: { ...DEFAULT_OUTAGES },
    });
    setShowModal(false);
    setNewControllerName("");
    loadControllers();
  };

  const handleControllerChange = async (id) => {
    if (dirty && !window.confirm("You have unsaved changes. Continue?")) return;
    setSelectedController(id);
    if (id) {
      loadSchedule(id);
    } else {
      setSchedule({ lights: [], outages: { ...DEFAULT_OUTAGES } });
    }
  };

  const discardChanges = async () => {
    if (!selectedController) return;
    await loadSchedule(selectedController);
    setDirty(false);
  };

  useEffect(() => {
    loadControllers();
  }, []);

  return (
    <div className="min-h-screen bg-white text-gray-900 dark:bg-gray-900 dark:text-gray-100 p-6 sm:px-8 px-4">
      <h1 className="text-3xl font-bold mb-6">🚦 Traffic Light Scheduler</h1>

      <div className="mb-6">
        <div className="flex flex-col sm:flex-row sm:items-center sm:gap-4">
          <select
            value={selectedController}
            onChange={(e) => handleControllerChange(e.target.value)}
            className="mb-2 sm:mb-0 bg-gray-100 border border-gray-300 rounded px-3 py-2 text-gray-900 dark:bg-gray-800 dark:border-gray-700 dark:text-gray-100"
          >
            <option value="">-- Select Controller --</option>
            {controllers.map((id) => (
              <option key={id} value={id}>
                {id}
              </option>
            ))}
          </select>
          <div className="flex gap-2">
            <button
              className="bg-blue-600 text-white px-4 py-2 rounded shadow hover:bg-blue-700"
              onClick={() => {
                if (
                  dirty &&
                  !window.confirm("You have unsaved changes. Continue?")
                )
                  return;
                setShowModal(true);
              }}
            >
              + Add Controller
            </button>
            <button
              className="bg-red-600 text-white px-4 py-2 rounded shadow hover:bg-red-700"
              onClick={deleteController}
            >
              Delete Controller
            </button>
          </div>
        </div>
      </div>

      {selectedController && (
        <div>
          {/* Outages Settings */}
          <div className="mb-6 border border-gray-300 dark:border-gray-700 rounded p-4 flex flex-col sm:flex-row gap-4">
            <label className="flex items-center gap-2">
              <input
                type="checkbox"
                checked={schedule.outages.enabled}
                onChange={(e) => {
                  setSchedule((s) => ({
                    ...s,
                    outages: { ...s.outages, enabled: e.target.checked },
                  }));
                  setDirty(true);
                }}
              />
              Enable Outages
            </label>
            {schedule.outages.enabled && (
              <div className="flex flex-col sm:flex-row gap-4">
                <div>
                  <label className="block text-sm">Period (s)</label>
                  <input
                    type="number"
                    value={schedule.outages.period}
                    onChange={(e) => {
                      setSchedule((s) => ({
                        ...s,
                        outages: {
                          ...s.outages,
                          period: Number(e.target.value),
                        },
                      }));
                      setDirty(true);
                    }}
                    className="bg-gray-100 border border-gray-300 rounded w-full px-3 py-1 dark:bg-gray-800 dark:border-gray-700"
                  />
                </div>
                <div>
                  <label className="block text-sm">Duration (s)</label>
                  <input
                    type="number"
                    value={schedule.outages.duration}
                    onChange={(e) => {
                      setSchedule((s) => ({
                        ...s,
                        outages: {
                          ...s.outages,
                          duration: Number(e.target.value),
                        },
                      }));
                      setDirty(true);
                    }}
                    className="bg-gray-100 border border-gray-300 rounded w-full px-3 py-1 dark:bg-gray-800 dark:border-gray-700"
                  />
                </div>
              </div>
            )}
          </div>

          {/* Lights */}
          {schedule.lights.map((light, lightIdx) => {
            const totalDuration = light.phases.reduce(
              (sum, ph) => sum + (ph.duration || 0),
              0,
            );

            return (
              <div key={lightIdx} className="mb-6">
                <h2 className="text-xl font-semibold mb-2 flex justify-between items-center">
                  <span>Light {lightIdx + 1}</span>
                  <span className="text-sm text-gray-600 dark:text-gray-400">
                    Total: {totalDuration}s
                  </span>
                </h2>
                <PhaseTable
                  phases={light.phases}
                  onChange={(phases) => updateLightPhases(lightIdx, phases)}
                  onAddPhase={() => addPhaseToLight(lightIdx)}
                />
                <button
                  className="bg-red-600 text-white px-3 py-1 rounded mt-2 hover:bg-red-700"
                  onClick={() => deleteLight(lightIdx)}
                >
                  Delete Light
                </button>
              </div>
            );
          })}

          {/* Controls */}
          <div className="flex gap-2 mt-4 flex-wrap">
            <button
              className="bg-purple-600 text-white px-4 py-2 rounded shadow hover:bg-purple-700"
              onClick={addLight}
            >
              + Add Light
            </button>
            <button
              className="bg-blue-600 text-white px-4 py-2 rounded shadow hover:bg-blue-700"
              onClick={saveSchedule}
            >
              Save
            </button>
            <button
              className="bg-gray-500 text-white px-4 py-2 rounded shadow hover:bg-gray-600"
              onClick={discardChanges}
            >
              Discard
            </button>
          </div>
        </div>
      )}

      {/* Modal */}
      {showModal && (
        <div
          className="fixed inset-0 flex items-center justify-center bg-black/80"
          onClick={() => setShowModal(false)}
        >
          <div
            className="bg-white dark:bg-gray-800 p-6 rounded-lg shadow w-80"
            onClick={(e) => e.stopPropagation()}
          >
            <h3 className="text-lg font-semibold mb-4">New Controller</h3>
            <input
              type="text"
              placeholder="Controller name"
              value={newControllerName}
              onChange={(e) => setNewControllerName(e.target.value)}
              className="bg-gray-50 border border-gray-300 rounded w-full px-3 py-2 mb-4 dark:bg-gray-900 dark:border-gray-700"
            />
            <div className="flex justify-end gap-2">
              <button
                className="bg-gray-400 dark:bg-gray-600 px-4 py-2 rounded hover:bg-gray-500"
                onClick={() => setShowModal(false)}
              >
                Cancel
              </button>
              <button
                className="bg-blue-600 text-white px-4 py-2 rounded hover:bg-blue-700"
                onClick={createController}
              >
                Create
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;
