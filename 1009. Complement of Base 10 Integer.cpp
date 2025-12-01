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

# ===================== CONFIG =====================
xml_file = 'app/build/reports/jacoco/jacocoTsysUnitedStatesDebugCoverage/jacocoTsysUnitedStatesDebugCoverage.xml'

# USER INPUT → RELEASE COLUMN FORMAT
version = "1.0.0"        # Example: '1.0.0'
branch = "develop"       # Example: 'develop'
date_str = datetime.today().strftime("%Y-%m-%d")

RELEASE_ID = f"{date_str}_v{version}_{branch}"

# Output file
output_dir = "coverage_dashboard"
os.makedirs(output_dir, exist_ok=True)
MASTER_CSV = os.path.join(output_dir, f"{RELEASE_ID}.csv")

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
    return round((covered / total) * 100, 2) if total else 0, total

def format_pct(value):
    """Add % symbol for output"""
    return f"{value}%"

# ===================== MASTER CSV GENERATOR =====================

def generate_master_csv(root, out_file=MASTER_CSV):
    with open(out_file, "w", newline="") as f:
        writer = csv.writer(f)

        # Header
        writer.writerow([
            "Release", "Level", "Package", "Class", 
            "Total Classes", "Total Methods", "Total Lines", 
            "Covered Lines", "Missed Lines", "Covered Branches", "Missed Branches",
            "Instruction Coverage (%)", "Branch Coverage (%)", "Average Coverage (%)",
            "Status", "Priority"
        ])

        # 1️⃣ Summary row
        total_line_cov = total_line_missed = 0
        total_branch_cov = total_branch_missed = 0

        for counter in root.findall("counter"):
            ctype, covered, missed = counter.get("type"), int(counter.get("covered")), int(counter.get("missed"))
            if ctype == "LINE":
                total_line_cov += covered
                total_line_missed += missed
            elif ctype == "BRANCH":
                total_branch_cov += covered
                total_branch_missed += missed

        total_elements = total_line_cov + total_line_missed
        total_cov_pct = round((total_line_cov / total_elements) * 100, 2) if total_elements else 0
        status, priority = get_status(total_cov_pct)

        writer.writerow([
            RELEASE_ID, "summary", "", "",
            "", "", "",
            total_line_cov, total_line_missed, total_branch_cov, total_branch_missed,
            format_pct(total_cov_pct), format_pct(total_cov_pct), format_pct(total_cov_pct),
            status, priority
        ])

        # 2️⃣ Package and Class rows
        for package in root.findall("package"):
            pkg_name = package.get("name")

            # Package-level aggregation
            class_count = 0
            method_count = 0
            line_total = 0
            instr_cov = instr_total = 0
            branch_cov = branch_total = 0

            for cls in package.findall("class"):
                class_name = cls.get("name").split("/")[-1]
                # Skip inner / anonymous classes
                if "$" in class_name:
                    continue

                class_count += 1
                method_count += len(cls.findall("method"))

                # Count per class
                class_instr_cov = class_instr_total = 0
                class_branch_cov = class_branch_total = 0
                class_lines = 0

                for counter in cls.findall("counter"):
                    ctype, covered, missed = counter.get("type"), int(counter.get("covered")), int(counter.get("missed"))
                    if ctype == "INSTRUCTION":
                        class_instr_cov += covered
                        class_instr_total += covered + missed
                    elif ctype == "BRANCH":
                        class_branch_cov += covered
                        class_branch_total += covered + missed
                    elif ctype == "LINE":
                        class_lines += covered + missed

                instr_cov += class_instr_cov
                instr_total += class_instr_total
                branch_cov += class_branch_cov
                branch_total += class_branch_cov + (class_branch_total - class_branch_cov)
                line_total += class_lines

                # Class-level percentages
                instr_pct, _ = safe_pct(class_instr_cov, class_instr_total - class_instr_cov)
                branch_pct, _ = safe_pct(class_branch_cov, class_branch_total - class_branch_cov)
                avg_pct = round((instr_pct + branch_pct) / 2, 2)
                status, priority = get_status(avg_pct)

                # Write class row
                writer.writerow([
                    RELEASE_ID, "class", pkg_name, class_name,
                    "", len(cls.findall("method")), class_lines,
                    "", "", "", "",
                    format_pct(instr_pct), format_pct(branch_pct), format_pct(avg_pct),
                    status, priority
                ])

            # Package-level percentages
            pkg_instr_pct, _ = safe_pct(instr_cov, instr_total - instr_cov)
            pkg_branch_pct, _ = safe_pct(branch_cov, branch_total - branch_cov)
            pkg_avg_pct = round((pkg_instr_pct + pkg_branch_pct) / 2, 2)
            status, priority = get_status(pkg_avg_pct)

            # Write package row
            writer.writerow([
                RELEASE_ID, "package", pkg_name, "",
                class_count, method_count, line_total,
                "", "", "", "",
                format_pct(pkg_instr_pct), format_pct(pkg_branch_pct), format_pct(pkg_avg_pct),
                status, priority
            ])

    print(f"✅ Master CSV generated: {out_file}")

# ===================== Run Script =====================

if __name__ == "__main__":
    tree = ET.parse(xml_file)
    root = tree.getroot()
    generate_master_csv(root)
