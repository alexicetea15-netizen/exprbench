import csv
import math
import statistics
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]

INPUT_FILE = ROOT / "results" / "benchmark.csv"
SUMMARY_FILE = ROOT / "results" / "benchmark_summary.csv"
SVG_FILE = ROOT / "results" / "benchmark.svg"


def read_results():
    if not INPUT_FILE.exists():
        raise FileNotFoundError(
            f"Benchmark results not found: {INPUT_FILE}"
        )

    grouped = defaultdict(list)

    with INPUT_FILE.open(
        "r", newline="", encoding="utf-8"
    ) as file:
        reader = csv.DictReader(file)

        required = {
            "operations",
            "run",
            "avg_cpu_microseconds",
        }

        if not reader.fieldnames or not required.issubset(
            reader.fieldnames
        ):
            raise ValueError("Unexpected benchmark CSV columns")

        for row in reader:
            operations = int(row["operations"])
            elapsed = float(row["avg_cpu_microseconds"])

            if operations <= 0 or elapsed < 0:
                raise ValueError("Invalid benchmark measurement")

            grouped[operations].append(elapsed)

    if not grouped:
        raise ValueError("No benchmark measurements found")

    return grouped


def write_summary(grouped):
    rows = []

    for operations in sorted(grouped):
        values = grouped[operations]

        rows.append({
            "operations": operations,
            "runs": len(values),
            "mean_cpu_microseconds": statistics.mean(values),
            "min_cpu_microseconds": min(values),
            "max_cpu_microseconds": max(values),
        })

    with SUMMARY_FILE.open(
        "w", newline="", encoding="utf-8"
    ) as file:
        writer = csv.DictWriter(
            file,
            fieldnames=[
                "operations",
                "runs",
                "mean_cpu_microseconds",
                "min_cpu_microseconds",
                "max_cpu_microseconds",
            ],
        )

        writer.writeheader()

        for row in rows:
            formatted = dict(row)

            for key in (
                "mean_cpu_microseconds",
                "min_cpu_microseconds",
                "max_cpu_microseconds",
            ):
                formatted[key] = f"{row[key]:.9f}"

            writer.writerow(formatted)

    return rows


def write_svg(rows):
    width = 1000
    height = 620

    left = 110
    right = 55
    top = 90
    bottom = 100

    plot_width = width - left - right
    plot_height = height - top - bottom

    operations = [row["operations"] for row in rows]
    means = [row["mean_cpu_microseconds"] for row in rows]

    x_min = math.log10(min(operations))
    x_max = math.log10(max(operations))

    if x_max == x_min:
        x_max = x_min + 1

    y_max = max(means) * 1.15

    if y_max <= 0:
        y_max = 1

    def x_position(value):
        return left + (
            (math.log10(value) - x_min) / (x_max - x_min)
        ) * plot_width

    def y_position(value):
        return top + plot_height - (
            value / y_max
        ) * plot_height

    parts = [
        f'<svg xmlns="http://www.w3.org/2000/svg" '
        f'width="{width}" height="{height}" '
        f'viewBox="0 0 {width} {height}">',
        '<rect width="100%" height="100%" fill="white"/>',
        '<style>',
        'text { font-family: Arial, sans-serif; fill: #222; }',
        '.grid { stroke: #ddd; stroke-width: 1; }',
        '.axis { stroke: #333; stroke-width: 1.5; }',
        '.line { fill: none; stroke: #2463a6; stroke-width: 3; }',
        '.point { fill: #2463a6; }',
        '</style>',
        '<text x="500" y="38" text-anchor="middle" '
        'font-size="22" font-weight="bold">'
        'ExprBench: Mean CPU Time</text>',
    ]

    # Horizontal grid lines and y-axis labels.
    for index in range(6):
        value = y_max * index / 5
        y = y_position(value)

        parts.append(
            f'<line x1="{left}" y1="{y:.2f}" '
            f'x2="{left + plot_width}" y2="{y:.2f}" '
            'class="grid"/>'
        )

        parts.append(
            f'<text x="{left - 12}" y="{y + 5:.2f}" '
            f'text-anchor="end" font-size="13">'
            f'{value:.3f}</text>'
        )

    # Axes.
    parts.append(
        f'<line x1="{left}" y1="{top}" '
        f'x2="{left}" y2="{top + plot_height}" class="axis"/>'
    )

    parts.append(
        f'<line x1="{left}" y1="{top + plot_height}" '
        f'x2="{left + plot_width}" y2="{top + plot_height}" '
        'class="axis"/>'
    )

    # X-axis labels.
    for value in operations:
        x = x_position(value)
        y = top + plot_height

        parts.append(
            f'<line x1="{x:.2f}" y1="{y}" '
            f'x2="{x:.2f}" y2="{y + 6}" class="axis"/>'
        )

        parts.append(
            f'<text x="{x:.2f}" y="{y + 25}" '
            f'text-anchor="middle" font-size="13">{value}</text>'
        )

    # Mean-time line.
    points = " ".join(
        f"{x_position(row['operations']):.2f},"
        f"{y_position(row['mean_cpu_microseconds']):.2f}"
        for row in rows
    )

    parts.append(f'<polyline points="{points}" class="line"/>')

    for row in rows:
        x = x_position(row["operations"])
        y = y_position(row["mean_cpu_microseconds"])

        parts.append(
            f'<circle cx="{x:.2f}" cy="{y:.2f}" r="5" '
            'class="point"/>'
        )

    # Axis titles.
    parts.append(
        f'<text x="{left + plot_width / 2}" '
        f'y="{height - 25}" text-anchor="middle" font-size="16">'
        'Expression size (operations, logarithmic scale)</text>'
    )

    parts.append(
        f'<text x="28" y="{top + plot_height / 2}" '
        'text-anchor="middle" font-size="16" '
        'transform="rotate(-90 28 '
        f'{top + plot_height / 2})">'
        'Mean CPU time (microseconds per evaluation)</text>'
    )

    parts.append('</svg>')

    SVG_FILE.write_text(
        "\n".join(parts) + "\n",
        encoding="utf-8",
    )


def main():
    grouped = read_results()
    rows = write_summary(grouped)
    write_svg(rows)

    print(f"Summary saved to: {SUMMARY_FILE}")
    print(f"Chart saved to: {SVG_FILE}")
    print()
    print("operations | mean CPU us | min CPU us | max CPU us")

    for row in rows:
        print(
            f"{row['operations']:10d} | "
            f"{row['mean_cpu_microseconds']:11.6f} | "
            f"{row['min_cpu_microseconds']:10.6f} | "
            f"{row['max_cpu_microseconds']:10.6f}"
        )


if __name__ == "__main__":
    main()
