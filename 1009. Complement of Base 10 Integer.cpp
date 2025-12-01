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

xml_file = 'app/build/reports/jacoco/jacocoTsysUnitedStatesDebugCoverage/jacocoTsysUnitedStatesDebugCoverage.xml'
output_file = "coverage_dashboard/coverage_unified.csv"


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
    return round((covered / total) * 100, 2) if total else 0


def fmt(pct):
    return f"{pct}%"


# ===================== MAIN UNIFIED CSV =====================

def generate_unified_csv(root):
    with open(output_file, "w", newline="") as f:
        writer = csv.writer(f)

        # Final unified header
        writer.writerow([
            "Level", "Package", "Class", "Metric",
            "Covered", "Missed",
            "Instruction (%)", "Branch (%)", "Average (%)",
            "Status", "Priority"
        ])

        # ---------- SUMMARY ----------
        total_line_cov = total_line_missed = 0
        total_branch_cov = total_branch_missed = 0

        for counter in root.findall("counter"):
            ctype = counter.get("type")
            covered = int(counter.get("covered"))
            missed = int(counter.get("missed"))

            if ctype == "LINE":
                total_line_cov += covered
                total_line_missed += missed
            elif ctype == "BRANCH":
                total_branch_cov += covered
                total_branch_missed += missed

        total_pct = safe_pct(total_line_cov, total_line_missed)
        status, priority = get_status(total_pct)

        writer.writerow([
            "summary", "ALL", "ALL", "Overall",
            total_line_cov, total_line_missed,
            "", "", fmt(total_pct),
            status, priority
        ])

        # ---------- PACKAGE LEVEL ----------
        for package in root.findall("package"):
            pkg_name = package.get("name")

            instr_cov = instr_total = branch_cov = branch_total = 0
            line_cov = line_total = 0
            method_count = 0

            for cls in package.findall("class"):

                method_count += len(cls.findall("method"))

                for counter in cls.findall("counter"):
                    ctype = counter.get("type")
                    covered = int(counter.get("covered"))
                    missed = int(counter.get("missed"))

                    if ctype == "INSTRUCTION":
                        instr_cov += covered
                        instr_total += covered + missed
                    elif ctype == "BRANCH":
                        branch_cov += covered
                        branch_total += covered + missed
                    elif ctype == "LINE":
                        line_total += covered + missed
                        line_cov += covered

            instr_pct = safe_pct(instr_cov, instr_total - instr_cov)
            branch_pct = safe_pct(branch_cov, branch_total - branch_cov)
            avg_pct = round((instr_pct + branch_pct) / 2, 2)

            status, priority = get_status(avg_pct)

            writer.writerow([
                "package", pkg_name, "",
                "Package Coverage",
                line_cov, line_total - line_cov,
                fmt(instr_pct), fmt(branch_pct), fmt(avg_pct),
                status, priority
            ])

        # ---------- CLASS LEVEL ----------
        for package in root.findall("package"):
            pkg_name = package.get("name")

            for cls in package.findall("class"):
                class_name = cls.get("name").split("/")[-1]

                instr_cov = instr_total = branch_cov = branch_total = 0
                line_cov = line_total = 0

                for counter in cls.findall("counter"):
                    ctype = counter.get("type")
                    covered = int(counter.get("covered"))
                    missed = int(counter.get("missed"))

                    if ctype == "INSTRUCTION":
                        instr_cov += covered
                        instr_total += covered + missed
                    elif ctype == "BRANCH":
                        branch_cov += covered
                        branch_total += covered + missed
                    elif ctype == "LINE":
                        line_total += covered + missed
                        line_cov += covered

                instr_pct = safe_pct(instr_cov, instr_total - instr_cov)
                branch_pct = safe_pct(branch_cov, branch_total - branch_cov)
                avg_pct = round((instr_pct + branch_pct) / 2, 2)

                status, priority = get_status(avg_pct)

                writer.writerow([
                    "class", pkg_name, class_name,
                    "Class Coverage",
                    line_cov, line_total - line_cov,
                    fmt(instr_pct), fmt(branch_pct), fmt(avg_pct),
                    status, priority
                ])

    print(f"✅ Unified CSV created at: {output_file}")


# ===================== EXECUTE =====================

if __name__ == "__main__":
    tree = ET.parse(xml_file)
    root = tree.getroot()

    generate_unified_csv(root)
