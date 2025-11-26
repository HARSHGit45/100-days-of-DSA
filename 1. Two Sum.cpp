package com.imobile3.pos.data.module.batch.close

import org.junit.Before
import org.junit.Test
import org.mockito.Mockito.*

class BatchCloseTenderCallbacksTest {

    private lateinit var batchCloseManager: BatchCloseManagerApi
    private lateinit var callbacks: BatchCloseTenderCallbacks

    @Before
    fun setUp() {
        batchCloseManager = mock(BatchCloseManagerApi::class.java)
        callbacks = BatchCloseTenderCallbacks(batchCloseManager)
    }

    @Test
    fun `ActionInProgress should call onTenderActionsSuccess`() {
        callbacks.ActionInProgress(1, 10)
        verify(batchCloseManager).onTenderActionsSuccess()
    }

    @Test
    fun `ActionOnComplete should call onTenderActionsSuccess`() {
        callbacks.ActionOnComplete(10, 10)
        verify(batchCloseManager).onTenderActionsSuccess()
    }

    @Test
    fun `ActionOnCancelled should call onFailure`() {
        callbacks.ActionOnCancelled()
        verify(batchCloseManager).onFailure("ActionOnCancelled")
    }

    @Test
    fun `ActionOnBusy should call onFailure`() {
        callbacks.ActionOnBusy()
        verify(batchCloseManager).onFailure("ActionOnBusy")
    }

    @Test
    fun `ActionOnBatchNotOpen should call onTenderActionsSuccess`() {
        callbacks.ActionOnBatchNotOpen()
        verify(batchCloseManager).onTenderActionsSuccess()
    }

    @Test
    fun `ActionOnNoActionableTenders should call onTenderActionsSuccess`() {
        callbacks.ActionOnNoActionableTenders()
        verify(batchCloseManager).onTenderActionsSuccess()
    }

    @Test
    fun `ActionOnConnectionError should call onFailure`() {
        callbacks.ActionOnConnectionError()
        verify(batchCloseManager).onFailure("ActionOnConnectionError")
    }

    @Test
    fun `ActionOnError should call onFailure`() {
        callbacks.ActionOnError()
        verify(batchCloseManager).onFailure("ActionOnError")
    }
}



class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n=nums.size();


        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)
            {
                if(nums[i]+nums[j]==target)
                {
                    return {i,j};
                }
            }
        }

        return {};
        
    }

}<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Coverage Dashboard</title>
    <link rel="stylesheet" href="style.css" />
</head>
<body>
    <div class="tabs">
        <button id="tabDashboard" class="active">Dashboard</button>
        <button id="tabCompare">Compare Releases</button>
    </div>

    <input type="text" id="searchBox" placeholder="Search packages/classes/methods..." />

    <button id="backBtn" style="display:none">⬅ Back</button>

    <table id="dataTable"></table>

    <script src="script.js"></script>
</body>
</html>


// -------------------------------
// FULL script.js - Dashboard + Full Comparison (Option B)
// -------------------------------

// GLOBAL STATE
let packageData = [];
let classData = [];
let methodData = [];

let oldPackageData = [];
let oldClassData = [];
let oldMethodData = [];

let currentLevel = "package"; // 'package'|'class'|'method'|'comp-package'|'comp-class'|'comp-method'
let currentPackage = null;
let currentClass = null;

let currentSort = { key: null, asc: true }; // used across views
let inComparison = false;

// Utility: safe parse to number (returns NaN if not numeric)
function toNum(v) {
  if (v === null || v === undefined || v === "") return NaN;
  // parseFloat handles decimals; trim first
  return parseFloat(String(v).trim());
}

// -------------------------------
// CSV LOADING (simple split-by-comma loader)
// - Assumes first row is header
// - Returns array of objects keyed by header
// -------------------------------
async function fetchText(path) {
  const res = await fetch(path);
  if (!res.ok) throw new Error(`Failed to load ${path}: ${res.status}`);
  return await res.text();
}

function parseCSVText(text) {
  const lines = text.split(/\r?\n/).filter(l => l.trim() !== "");
  if (lines.length === 0) return [];
  const headers = lines[0].split(",").map(h => h.trim());
  const rows = [];
  for (let i = 1; i < lines.length; i++) {
    const cols = lines[i].split(",").map(c => c.trim());
    const obj = {};
    for (let j = 0; j < headers.length; j++) obj[headers[j]] = cols[j] ?? "";
    rows.push(obj);
  }
  return rows;
}

async function loadAllCSVs() {
  try {
    const [
      pkgTxt, clsTxt, metTxt,
      oldPkgTxt, oldClsTxt, oldMetTxt
    ] = await Promise.all([
      fetchText("package_report.csv"),
      fetchText("class_report.csv"),
      fetchText("method_report.csv"),
      fetchText("package_report.csv"),
      fetchText("class_report.csv"),
      fetchText("method_report.csv")
    ]);

    const pkgRows = parseCSVText(pkgTxt);
    const clsRows = parseCSVText(clsTxt);
    const metRows = parseCSVText(metTxt);

    const oldPkgRows = parseCSVText(oldPkgTxt);
    const oldClsRows = parseCSVText(oldClsTxt);
    const oldMetRows = parseCSVText(oldMetTxt);

    // Normalize current data into simpler keys we use
    packageData = pkgRows.map(r => ({
      package: r["Package"] ?? r["package"] ?? "",
      classes: toNum(r["Total Classes"] ?? r["Classes"] ?? r["total_classes"] ?? ""),
      methods: toNum(r["Total Methods"] ?? r["methods"] ?? ""),
      lines: toNum(r["Total Lines"] ?? r["Lines"] ?? ""),
      inst: toNum(r["Instruction Coverage (%)"] ?? r["Instruction %"] ?? r["inst"] ?? ""),
      branch: toNum(r["Branch Coverage (%)"] ?? r["Branch %"] ?? r["branch"] ?? ""),
      avg: toNum(r["Average Coverage (%)"] ?? r["Average %"] ?? r["avg"] ?? ""),
      status: r["Status"] ?? ""
    }));

    classData = clsRows.map(r => ({
      package: r["Package"] ?? r["package"] ?? "",
      class: r["Class"] ?? r["class"] ?? "",
      methods: toNum(r["Total Methods"] ?? r["methods"] ?? ""),
      lines: toNum(r["Total Lines"] ?? r["Lines"] ?? ""),
      inst: toNum(r["Instruction Coverage (%)"] ?? r["Instruction %"] ?? r["inst"] ?? ""),
      branch: toNum(r["Branch Coverage (%)"] ?? r["Branch %"] ?? r["branch"] ?? ""),
      avg: toNum(r["Average Coverage (%)"] ?? r["Average %"] ?? r["avg"] ?? ""),
      status: r["Status"] ?? ""
    }));

    methodData = metRows.map(r => ({
      package: r["Package"] ?? r["package"] ?? "",
      class: r["Class"] ?? r["class"] ?? "",
      method: r["Method"] ?? r["method"] ?? "",
      line: r["Line Number"] ?? r["Line"] ?? r["line"] ?? "",
      inst: toNum(r["Instruction Coverage (%)"] ?? r["Instruction %"] ?? r["inst"] ?? ""),
      branch: toNum(r["Branch Coverage (%)"] ?? r["Branch %"] ?? r["branch"] ?? ""),
      avg: toNum(r["Average Coverage (%)"] ?? r["Average %"] ?? r["avg"] ?? ""),
      status: r["Status"] ?? ""
    }));

    // Old data
    oldPackageData = oldPkgRows.map(r => ({
      package: r["Package"] ?? r["package"] ?? "",
      inst: toNum(r["Instruction Coverage (%)"] ?? r["Instruction %"] ?? r["inst"] ?? ""),
      branch: toNum(r["Branch Coverage (%)"] ?? r["Branch %"] ?? r["branch"] ?? ""),
      avg: toNum(r["Average Coverage (%)"] ?? r["Average %"] ?? r["avg"] ?? "")
    }));

    oldClassData = oldClsRows.map(r => ({
      package: r["Package"] ?? r["package"] ?? "",
      class: r["Class"] ?? r["class"] ?? "",
      inst: toNum(r["Instruction Coverage (%)"] ?? r["Instruction %"] ?? r["inst"] ?? ""),
      branch: toNum(r["Branch Coverage (%)"] ?? r["Branch %"] ?? r["branch"] ?? ""),
      avg: toNum(r["Average Coverage (%)"] ?? r["Average %"] ?? r["avg"] ?? "")
    }));

    oldMethodData = oldMetRows.map(r => ({
      package: r["Package"] ?? r["package"] ?? "",
      class: r["Class"] ?? r["class"] ?? "",
      method: r["Method"] ?? r["method"] ?? "",
      inst: toNum(r["Instruction Coverage (%)"] ?? r["Instruction %"] ?? r["inst"] ?? ""),
      branch: toNum(r["Branch Coverage (%)"] ?? r["Branch %"] ?? r["branch"] ?? r["branch"] ?? ""),
      avg: toNum(r["Average Coverage (%)"] ?? r["Average %"] ?? r["avg"] ?? "")
    }));

    // initial render
    renderPackages();
  } catch (err) {
    console.error(err);
    alert("Failed to load CSVs. Check console for errors.");
  }
}

// -------------------------------
// SEARCH FILTER
// -------------------------------
function applySearchFilter(data) {
  const qEl = document.getElementById("searchBox");
  const q = qEl ? String(qEl.value || "").toLowerCase().trim() : "";
  if (!q) return data;
  return data.filter(row => Object.values(row).some(v => String(v).toLowerCase().includes(q)));
}

// -------------------------------
// SORTING (numeric-aware)
// - sortData mutates array (like Array.sort) and returns it
// -------------------------------
function sortData(data, key, asc = true) {
  return data.sort((a, b) => {
    const A = a[key];
    const B = b[key];
    const nA = toNum(A);
    const nB = toNum(B);
    if (!isNaN(nA) && !isNaN(nB)) return asc ? nA - nB : nB - nA;
    const sA = String(A ?? "").toLowerCase();
    const sB = String(B ?? "").toLowerCase();
    return asc ? sA.localeCompare(sB) : sB.localeCompare(sA);
  });
}

// -------------------------------
// RENDER TABLE (generic)
// -------------------------------
function renderTable(columns, rows, clickHandler = null) {
  const table = document.getElementById("dataTable");
  if (!table) {
    console.error("dataTable element not found");
    return;
  }
  table.innerHTML = "";

  const thead = document.createElement("thead");
  const tr = document.createElement("tr");
  for (const col of columns) {
    const th = document.createElement("th");
    th.textContent = col.label;
    th.style.cursor = "pointer";
    th.onclick = () => {
      // Special handling: when in comparison views default sort is descending by delta if key starts with 'delta'
      if (inComparison && col.key.startsWith("delta_") && currentSort.key !== col.key) {
        currentSort = { key: col.key, asc: false }; // default DESC for delta
      } else if (currentSort.key === col.key) {
        currentSort.asc = !currentSort.asc;
      } else {
        currentSort = { key: col.key, asc: true };
      }
      // Apply sort on visible rows (re-render via current view)
      renderSorted(col.key);
    };
    tr.appendChild(th);
  }
  thead.appendChild(tr);
  table.appendChild(thead);

  const tbody = document.createElement("tbody");
  for (const row of rows) {
    const trRow = document.createElement("tr");
    if (clickHandler) trRow.onclick = () => clickHandler(row);
    for (const col of columns) {
      const td = document.createElement("td");
      // allow HTML in cell if user provided (like colored spans)
      td.innerHTML = row[col.key] === undefined ? "" : row[col.key];
      trRow.appendChild(td);
    }
    tbody.appendChild(trRow);
  }
  table.appendChild(tbody);
}

// -------------------------------
// DASHBOARD VIEWS (unchanged behavior)
// -------------------------------
function renderPackages(sortKey = null) {
  inComparison = false;
  currentLevel = "package";
  currentPackage = null;
  currentClass = null;
  document.getElementById("backBtn").style.display = "none";

  let rows = packageData.map(r => ({ ...r })); // shallow copy
  if (sortKey) {
    // toggle behavior handled by renderSorted
    rows = sortData(rows, sortKey, currentSort.asc);
  } else if (currentSort.key && currentSort.key !== null) {
    rows = sortData(rows, currentSort.key, currentSort.asc);
  }

  rows = applySearchFilter(rows);

  renderTable([
    { key: "package", label: "Package" },
    { key: "classes", label: "Total Classes" },
    { key: "inst", label: "Instruction %" },
    { key: "branch", label: "Branch %" },
    { key: "avg", label: "Average %" },
    { key: "status", label: "Status" }
  ], rows, row => renderClasses(row.package));
}

function renderClasses(pkgName, sortKey = null) {
  inComparison = false;
  currentLevel = "class";
  currentPackage = pkgName;
  currentClass = null;
  document.getElementById("backBtn").style.display = "inline-block";

  let rows = classData.filter(r => r.package === pkgName).map(r => ({ ...r }));
  if (sortKey) rows = sortData(rows, sortKey, currentSort.asc);
  else if (currentSort.key) rows = sortData(rows, currentSort.key, currentSort.asc);

  rows = applySearchFilter(rows);

  renderTable([
    { key: "class", label: "Class" },
    { key: "methods", label: "Total Methods" },
    { key: "inst", label: "Instruction %" },
    { key: "branch", label: "Branch %" },
    { key: "avg", label: "Average %" },
    { key: "status", label: "Status" }
  ], rows, row => renderMethods(row.class));
}

function renderMethods(className, sortKey = null) {
  inComparison = false;
  currentLevel = "method";
  currentClass = className;
  document.getElementById("backBtn").style.display = "inline-block";

  let rows = methodData.filter(r => r.class === className).map(r => ({ ...r }));
  if (sortKey) rows = sortData(rows, sortKey, currentSort.asc);
  else if (currentSort.key) rows = sortData(rows, currentSort.key, currentSort.asc);

  rows = applySearchFilter(rows);

  renderTable([
    { key: "method", label: "Method" },
    { key: "line", label: "Line" },
    { key: "inst", label: "Instruction %" },
    { key: "branch", label: "Branch %" },
    { key: "avg", label: "Average %" },
    { key: "status", label: "Status" }
  ], rows);
}

// -------------------------------
// COMPARISON VIEWS (full delta at package/class/method level)
// -------------------------------
// Helper: lookup old values by package/class/method
const oldPackageMap = () => {
  const m = new Map();
  for (const r of oldPackageData) m.set(r.package, r);
  return m;
};
const oldClassMap = () => {
  const m = new Map();
  for (const r of oldClassData) m.set(`${r.package}||${r.class}`, r);
  return m;
};
const oldMethodMap = () => {
  const m = new Map();
  for (const r of oldMethodData) m.set(`${r.package}||${r.class}||${r.method}`, r);
  return m;
};

function renderComparisonPackages(sortKey = null) {
  inComparison = true;
  currentLevel = "comp-package";
  currentPackage = null;
  currentClass = null;
  document.getElementById("backBtn").style.display = "none";

  const prevMap = oldPackageMap();
  const changed = [];

  for (const cur of packageData) {
    const prev = prevMap.get(cur.package);
    if (!prev) continue;
    const di = Number((cur.inst - prev.inst).toFixed(2));
    const db = Number((cur.branch - prev.branch).toFixed(2));
    const da = Number((cur.avg - prev.avg).toFixed(2));
    if (di === 0 && db === 0 && da === 0) continue;
    changed.push({
      package: cur.package,
      // store numeric delta too for sorting if needed
      delta_inst: di,
      delta_branch: db,
      delta_avg: da,
      inst: `${cur.inst} <span class='${di>=0?'progress-up':'progress-down'}'>(${di>=0?'+':''}${di})</span>`,
      branch: `${cur.branch} <span class='${db>=0?'progress-up':'progress-down'}'>(${db>=0?'+':''}${db})</span>`,
      avg: `${cur.avg} <span class='${da>=0?'progress-up':'progress-down'}'>(${da>=0?'+':''}${da})</span>`
    });
  }

  // default sort: largest avg delta descending
  if (!sortKey) {
    changed.sort((a,b) => b.delta_avg - a.delta_avg);
    currentSort = { key: "delta_avg", asc: false };
  } else {
    // if user click to sort, use that key (supports delta_inst/delta_branch/delta_avg or inst/branch/avg)
    const asc = currentSort.key === sortKey ? currentSort.asc : (sortKey.startsWith("delta_") ? false : true);
    changed.sort((a,b) => {
      const A = a[sortKey], B = b[sortKey];
      if (typeof A === "number" && typeof B === "number") return asc ? A - B : B - A;
      return asc ? String(A).localeCompare(String(B)) : String(B).localeCompare(String(A));
    });
  }

  const rows = changed.map(r => ({
    package: r.package,
    inst: r.inst,
    branch: r.branch,
    avg: r.avg,
    delta_inst: r.delta_inst,
    delta_branch: r.delta_branch,
    delta_avg: r.delta_avg
  }));

  renderTable([
    { key: "package", label: "Package" },
    { key: "inst", label: "Instruction (cur ±Δ)" },
    { key: "branch", label: "Branch (cur ±Δ)" },
    { key: "avg", label: "Average (cur ±Δ)" }
  ], rows, row => renderComparisonClasses(row.package));
}

function renderComparisonClasses(pkgName, sortKey = null) {
  inComparison = true;
  currentLevel = "comp-class";
  currentPackage = pkgName;
  currentClass = null;
  document.getElementById("backBtn").style.display = "inline-block";

  const prevMap = oldClassMap();
  const changed = [];

  const classes = classData.filter(c => c.package === pkgName);
  for (const cur of classes) {
    const prev = prevMap.get(`${cur.package}||${cur.class}`);
    if (!prev) continue;
    const di = Number((cur.inst - prev.inst).toFixed(2));
    const db = Number((cur.branch - prev.branch).toFixed(2));
    const da = Number((cur.avg - prev.avg).toFixed(2));
    if (di === 0 && db === 0 && da === 0) continue;
    changed.push({
      class: cur.class,
      delta_inst: di,
      delta_branch: db,
      delta_avg: da,
      inst: `${cur.inst} <span class='${di>=0?'progress-up':'progress-down'}'>(${di>=0?'+':''}${di})</span>`,
      branch: `${cur.branch} <span class='${db>=0?'progress-up':'progress-down'}'>(${db>=0?'+':''}${db})</span>`,
      avg: `${cur.avg} <span class='${da>=0?'progress-up':'progress-down'}'>(${da>=0?'+':''}${da})</span>`
    });
  }

  // default sort descending by delta_avg
  if (!sortKey) {
    changed.sort((a,b) => b.delta_avg - a.delta_avg);
    currentSort = { key: "delta_avg", asc: false };
  } else {
    const asc = currentSort.key === sortKey ? currentSort.asc : (sortKey.startsWith("delta_") ? false : true);
    changed.sort((a,b) => {
      const A = a[sortKey], B = b[sortKey];
      if (typeof A === "number" && typeof B === "number") return asc ? A - B : B - A;
      return asc ? String(A).localeCompare(String(B)) : String(B).localeCompare(String(A));
    });
  }

  const rows = changed.map(r => ({
    class: r.class,
    inst: r.inst,
    branch: r.branch,
    avg: r.avg,
    delta_inst: r.delta_inst,
    delta_branch: r.delta_branch,
    delta_avg: r.delta_avg
  }));

  renderTable([
    { key: "class", label: "Class" },
    { key: "inst", label: "Instruction (cur ±Δ)" },
    { key: "branch", label: "Branch (cur ±Δ)" },
    { key: "avg", label: "Average (cur ±Δ)" }
  ], rows, row => renderComparisonMethods(pkgName, row.class));
}

function renderComparisonMethods(pkgName, className, sortKey = null) {
  inComparison = true;
  currentLevel = "comp-method";
  currentPackage = pkgName;
  currentClass = className;
  document.getElementById("backBtn").style.display = "inline-block";

  const prevMap = oldMethodMap();
  const changed = [];

  const methods = methodData.filter(m => m.package === pkgName && m.class === className);
  for (const cur of methods) {
    const prev = prevMap.get(`${cur.package}||${cur.class}||${cur.method}`);
    if (!prev) continue;
    const di = Number((cur.inst - prev.inst).toFixed(2));
    const db = Number((cur.branch - prev.branch).toFixed(2));
    const da = Number((cur.avg - prev.avg).toFixed(2));
    if (di === 0 && db === 0 && da === 0) continue;
    changed.push({
      method: cur.method,
      delta_inst: di,
      delta_branch: db,
      delta_avg: da,
      inst: `${cur.inst} <span class='${di>=0?'progress-up':'progress-down'}'>(${di>=0?'+':''}${di})</span>`,
      branch: `${cur.branch} <span class='${db>=0?'progress-up':'progress-down'}'>(${db>=0?'+':''}${db})</span>`,
      avg: `${cur.avg} <span class='${da>=0?'progress-up':'progress-down'}'>(${da>=0?'+':''}${da})</span>`
    });
  }

  // default sort descending by delta_avg
  if (!sortKey) {
    changed.sort((a,b) => b.delta_avg - a.delta_avg);
    currentSort = { key: "delta_avg", asc: false };
  } else {
    const asc = currentSort.key === sortKey ? currentSort.asc : (sortKey.startsWith("delta_") ? false : true);
    changed.sort((a,b) => {
      const A = a[sortKey], B = b[sortKey];
      if (typeof A === "number" && typeof B === "number") return asc ? A - B : B - A;
      return asc ? String(A).localeCompare(String(B)) : String(B).localeCompare(String(A));
    });
  }

  const rows = changed.map(r => ({
    method: r.method,
    inst: r.inst,
    branch: r.branch,
    avg: r.avg,
    delta_inst: r.delta_inst,
    delta_branch: r.delta_branch,
    delta_avg: r.delta_avg
  }));

  renderTable([
    { key: "method", label: "Method" },
    { key: "inst", label: "Instruction (cur ±Δ)" },
    { key: "branch", label: "Branch (cur ±Δ)" },
    { key: "avg", label: "Average (cur ±Δ)" }
  ], rows);
}

// -------------------------------
// RENDER SORTED CONTROLLER
// - detects whether current view is comparison vs dashboard
// - key provided is the column key from renderTable
// -------------------------------
function renderSorted(key) {
  // When key is a displayed delta cell (we used delta_* internally),
  // user clicking will toggle asc/desc. We map displayed keys to internal keys:
  if (inComparison) {
    // map clicked column keys to internal sort key if needed
    if (currentLevel === "comp-package") renderComparisonPackages(key);
    else if (currentLevel === "comp-class") renderComparisonClasses(currentPackage, key);
    else if (currentLevel === "comp-method") renderComparisonMethods(currentPackage, currentClass, key);
    else renderComparisonPackages(key);
  } else {
    if (currentLevel === "package") renderPackages(key);
    else if (currentLevel === "class") renderClasses(currentPackage, key);
    else if (currentLevel === "method") renderMethods(currentClass, key);
    else renderPackages(key);
  }
}

// -------------------------------
// TAB LOGIC (assumes elements exist)
// -------------------------------
document.getElementById("tabDashboard").onclick = () => {
  document.getElementById("tabDashboard").classList.add("active");
  document.getElementById("tabCompare").classList.remove("active");
  // reset current sort for dashboard if you prefer
  currentSort = { key: null, asc: true };
  renderPackages();
};

document.getElementById("tabCompare").onclick = () => {
  document.getElementById("tabCompare").classList.add("active");
  document.getElementById("tabDashboard").classList.remove("active");
  // default comparison landing page: package-level changes
  currentSort = { key: "delta_avg", asc: false };
  renderComparisonPackages();
};

// -------------------------------
// BACK BUTTON
// -------------------------------
document.getElementById("backBtn").onclick = () => {
  if (inComparison) {
    if (currentLevel === "comp-method") renderComparisonClasses(currentPackage);
    else if (currentLevel === "comp-class") renderComparisonPackages();
    else renderComparisonPackages();
  } else {
    if (currentLevel === "method") renderClasses(currentPackage);
    else if (currentLevel === "class") renderPackages();
    else renderPackages();
  }
};

// -------------------------------
// SEARCH BOX
// -------------------------------
document.getElementById("searchBox").addEventListener("input", () => {
  // keep current view, just re-render
  if (inComparison) {
    if (currentLevel === "comp-package") renderComparisonPackages();
    else if (currentLevel === "comp-class") renderComparisonClasses(currentPackage);
    else if (currentLevel === "comp-method") renderComparisonMethods(currentPackage, currentClass);
    else renderComparisonPackages();
  } else {
    if (currentLevel === "package") renderPackages();
    else if (currentLevel === "class") renderClasses(currentPackage);
    else if (currentLevel === "method") renderMethods(currentClass);
    else renderPackages();
  }
});

// -------------------------------
// INIT
// -------------------------------
window.onload = () => {
  loadAllCSVs();
};





body {
  font-family: Arial, sans-serif;
  margin: 20px;
  background: #f8f9fa;
}

.tabs {
  display: flex;
  gap: 10px;
  margin-bottom: 15px;
}

.tabs button {
  padding: 10px 18px;
  border: none;
  background: #ddd;
  border-radius: 6px;
  cursor: pointer;
}

.tabs button.active {
  background: #007bff;
  color: white;
}

#searchBox {
  width: 300px;
  padding: 8px;
  margin-bottom: 12px;
}

#backBtn {
  margin-bottom: 12px;
  padding: 8px 14px;
  cursor: pointer;
}

table {
  width: 100%;
  border-collapse: collapse;
  background: white;
}

table th, table td {
  padding: 10px;
  border: 1px solid #ccc;
  text-align: left;
}

table tr:hover {
  background: #f1f1f1;
  cursor: pointer;
}

.progress-up {
  color: green;
  font-weight: bold;
}

.progress-down {
  color: red;
  font-weight: bold;
}


