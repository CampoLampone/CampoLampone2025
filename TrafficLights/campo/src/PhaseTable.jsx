// PhaseTable.jsx
import React from "react";

export default function PhaseTable({
  phases,
  onChange,
  onAddPhase,
  onDeletePhase,
}) {
  const updatePhase = (idx, key, val) => {
    const newPhases = phases.map((p, i) =>
      i === idx ? { ...p, [key]: val } : p,
    );
    onChange(newPhases);
  };

  return (
    <div className="bg-gray-100 dark:bg-gray-800 shadow rounded p-4 mb-4">
      <div className="overflow-x-auto px-2 mb-2">
        <table className="table-auto w-full min-w-[320px]">
          <thead>
            <tr className="bg-gray-200 dark:bg-gray-700">
              <th className="px-3 py-2 text-left">#</th>
              <th className="px-3 py-2 text-left">Color</th>
              <th className="px-3 py-2 text-left">Duration (s)</th>
              <th className="px-3 py-2"></th>
            </tr>
          </thead>
          <tbody>
            {phases.map((ph, i) => (
              <tr
                key={i}
                className="border-b border-gray-300 dark:border-gray-700"
              >
                <td className="px-3 py-2">{i + 1}</td>
                <td className="px-3 py-2">
                  <select
                    value={ph.color}
                    onChange={(e) => updatePhase(i, "color", e.target.value)}
                    className="bg-gray-50 border border-gray-300 rounded px-2 py-1 text-gray-900 dark:bg-gray-900 dark:border-gray-700 dark:text-gray-100 max-w-[120px] w-full"
                  >
                    <option value="red">Red</option>
                    <option value="redyellow">Red/Yellow</option>
                    <option value="yellow">Yellow</option>
                    <option value="green">Green</option>
                  </select>
                </td>
                <td className="px-3 py-2">
                  <input
                    type="number"
                    value={ph.duration}
                    onChange={(e) =>
                      updatePhase(i, "duration", parseInt(e.target.value) || 1)
                    }
                    min="1"
                    className="bg-gray-50 border border-gray-300 rounded px-2 py-1 max-w-[80px] w-full dark:bg-gray-900 dark:border-gray-700"
                  />
                </td>
                <td className="px-3 py-2">
                  <button
                    className="bg-red-600 text-white px-2 py-1 rounded hover:bg-red-700"
                    onClick={() => onDeletePhase(i)}
                  >
                    ✕
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
      <button
        className="bg-green-600 text-white px-4 py-2 rounded shadow hover:bg-green-700"
        onClick={onAddPhase}
      >
        + Add Phase
      </button>
    </div>
  );
}
