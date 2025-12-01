#include <math.h>

class Solution {
public:
    int bitwiseComplement(int n) {


if(n==0)
    return 1;
        int mask=0;
        int m=n;


        while(m!=0)
        {
         mask=( mask<<1) | 1;
            m = m>>1;
        }

        int ans = (~n) & mask;

      


        return ans;


        
    }

};

import xml.etree.ElementTree as ET
import csv
import os
from datetime import datetime


# ===================== Helpers =====================

def get_status(avg_coverage):
    if avg_coverage < 20:
        return "Critical", "Immediate"
    elif 20 <= avg_coverage <= 59:
        return "Poor", "High"
    elif 60 <= avg_coverage <= 79:
        return "Fair", "High"
    elif 80 <= avg_coverage <= 89:
        return "Good", "Low"
    else:
        return "Excellent", "Low"


def safe_pct(covered, missed):
    total = covered + missed
    return (covered / total) * 100 if total else 0


# ===================== MAIN CSV GENERATOR =====================

def generate_unified_csv(root, release_name, out_dir="coverage_dashboard"):
    os.makedirs(out_dir, exist_ok=True)

    out_file = os.path.join(out_dir, f"{release_name}.csv")

    with open(out_file, "w", newline="") as f:
        writer = csv.writer(f)

        # ===================== SUMMARY =====================
        writer.writerow(["SECTION", "Metric", "Value"])

        total_line_cov = total_line_missed = 0
        total_branch_cov = total_branch_missed = 0

        for counter in root.findall("counter"):
            ctype = counter.get("type")
            covered, missed = int(counter.get("covered")), int(counter.get("missed"))
            if ctype == "LINE":
                total_line_cov += covered
                total_line_missed += missed
            elif ctype == "BRANCH":
                total_branch_cov += covered
                total_branch_missed += missed

        coverage_pct = safe_pct(total_line_cov, total_line_missed)
        status, priority = get_status(coverage_pct)

        writer.writerow(["SUMMARY", "Covered Lines", total_line_cov])
        writer.writerow(["SUMMARY", "Missed Lines", total_line_missed])
        writer.writerow(["SUMMARY", "Covered Branches", total_branch_cov])
        writer.writerow(["SUMMARY", "Missed Branches", total_branch_missed])
        writer.writerow(["SUMMARY", "Total Coverage (%)", f"{coverage_pct:.2f}%"])
        writer.writerow(["SUMMARY", "Status", status])
        writer.writerow(["SUMMARY", "Priority", priority])

        writer.writerow([])

        # ===================== PACKAGE LEVEL =====================

        writer.writerow([
            "SECTION", "Package", "Total Classes", "Total Methods", "Total Lines",
            "Instruction Coverage (%)", "Branch Coverage (%)",
            "Average Coverage (%)", "Status", "Priority"
        ])

        for package in root.findall("package"):
            pkg_name = package.get("name")

            class_count = len(package.findall("class"))
            total_methods = 0
            instr_cov = instr_total = branch_cov = branch_total = lines = 0

            for cls in package.findall("class"):
                for counter in cls.findall("counter"):
                    ctype = counter.get("type")
                    covered, missed = int(counter.get("covered")), int(counter.get("missed"))

                    if ctype == "INSTRUCTION":
                        instr_cov += covered
                        instr_total += covered + missed
                    elif ctype == "BRANCH":
                        branch_cov += covered
                        branch_total += covered + missed
                    elif ctype == "LINE":
                        lines += covered + missed

                total_methods += len(cls.findall("method"))

            instr_pct = safe_pct(instr_cov, instr_total - instr_cov)
            branch_pct = safe_pct(branch_cov, branch_total - branch_cov)
            avg_pct = (instr_pct + branch_pct) / 2
            status, priority = get_status(avg_pct)

            writer.writerow([
                "PACKAGE", pkg_name, class_count, total_methods, lines,
                f"{instr_pct:.2f}%", f"{branch_pct:.2f}%",
                f"{avg_pct:.2f}%", status, priority
            ])

        writer.writerow([])

        # ===================== CLASS LEVEL (AGGREGATED) =====================

        writer.writerow([
            "SECTION", "Package", "Class", "Total Methods", "Total Lines",
            "Instruction Coverage (%)", "Branch Coverage (%)",
            "Average Coverage (%)", "Status", "Priority"
        ])

        for package in root.findall("package"):
            pkg_name = package.get("name")

            parent_classes = {}

            for cls in package.findall("class"):
                raw = cls.get("name")
                if not raw:
                    continue

                short = raw.split("/")[-1]

                parent = short.split("$")[0]

                d = parent_classes.setdefault(parent, {
                    "methods": 0,
                    "instr_cov": 0,
                    "instr_total": 0,
                    "branch_cov": 0,
                    "branch_total": 0,
                    "lines": 0
                })

                d["methods"] += len(cls.findall("method"))

                for counter in cls.findall("counter"):
                    ctype = counter.get("type")
                    covered, missed = int(counter.get("covered")), int(counter.get("missed"))

                    if ctype == "INSTRUCTION":
                        d["instr_cov"] += covered
                        d["instr_total"] += covered + missed
                    elif ctype == "BRANCH":
                        d["branch_cov"] += covered
                        d["branch_total"] += covered + missed
                    elif ctype == "LINE":
                        d["lines"] += covered + missed

            for class_name, d in parent_classes.items():
                instr_pct = safe_pct(d["instr_cov"], d["instr_total"] - d["instr_cov"])
                branch_pct = safe_pct(d["branch_cov"], d["branch_total"] - d["branch_cov"])
                avg_pct = (instr_pct + branch_pct) / 2
                status, priority = get_status(avg_pct)

                writer.writerow([
                    "CLASS", pkg_name, class_name, d["methods"], d["lines"],
                    f"{instr_pct:.2f}%", f"{branch_pct:.2f}%",
                    f"{avg_pct:.2f}%", status, priority
                ])

    print(f"✅ Unified CSV created: {out_file}")


# ===================== MAIN =====================

if __name__ == "__main__":
    xml_file = 'app/build/reports/jacoco/jacocoTsysUnitedStatesDebugCoverage/jacocoTsysUnitedStatesDebugCoverage.xml'

    tree = ET.parse(xml_file)
    root = tree.getroot()

    # Example release name:
    # 2025-01-22_v1.4.0_develop
    release_name = datetime.now().strftime("%Y-%m-%d") + "_v1.4.0_develop"

    generate_unified_csv(root, release_name)


