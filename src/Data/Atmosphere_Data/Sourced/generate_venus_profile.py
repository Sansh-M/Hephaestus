"""Build the project's temporary one-dimensional Venus atmosphere profile.

The lower and middle atmosphere rows come from the Pioneer Venus Sounder
Probe table archived by the NASA PDS Atmospheres Node. The upper atmosphere
rows are the nominal Model I values in NASA SP-8011, Table 5.
"""

from __future__ import annotations

import csv
from pathlib import Path


SOURCE_DIR = Path(__file__).resolve().parent
PIONEER_SOURCE = SOURCE_DIR / "raw" / "venus" / "venustemppres.txt"
OUTPUT = SOURCE_DIR.parent / "VENUS.csv"

PROJECT_REFERENCE_RADIUS_KM = 6052.0
SP8011_REFERENCE_RADIUS_KM = 6050.0


# Altitude (km above the SP-8011 reference radius), temperature (K),
# pressure (millibar). These are the 120-350 km rows from Table 5, Model I.
SP8011_MODEL_I = (
    (120.0, 203.9, 1.59e-4),
    (130.0, 214.0, 1.91e-5),
    (140.0, 268.0, 3.01e-6),
    (150.0, 378.4, 7.79e-7),
    (160.0, 502.4, 2.98e-7),
    (170.0, 591.0, 1.41e-7),
    (180.0, 641.4, 7.51e-8),
    (190.0, 674.9, 4.28e-8),
    (200.0, 691.5, 2.58e-8),
    (210.0, 700.8, 1.62e-8),
    (220.0, 705.5, 1.06e-8),
    (230.0, 707.8, 7.20e-9),
    (240.0, 709.0, 5.07e-9),
    (250.0, 709.4, 3.69e-9),
    (260.0, 709.4, 2.75e-9),
    (270.0, 709.4, 2.10e-9),
    (280.0, 709.4, 1.64e-9),
    (290.0, 709.4, 1.30e-9),
    (300.0, 709.4, 1.05e-9),
    (310.0, 709.5, 8.51e-10),
    (320.0, 709.5, 7.01e-10),
    (330.0, 709.5, 5.82e-10),
    (340.0, 709.5, 4.89e-10),
    (350.0, 709.5, 4.14e-10),
)


def extract_numeric_rows(
    lines: list[str], start_marker: str, end_marker: str
) -> list[tuple[float, float, float]]:
    """Return altitude, pressure-bar, temperature rows from one PDS section."""
    start = next(i for i, line in enumerate(lines) if start_marker in line)
    end = next(
        i for i, line in enumerate(lines[start + 1 :], start + 1)
        if end_marker in line
    )

    rows: list[tuple[float, float, float]] = []
    for line in lines[start + 1 : end]:
        fields = line.split()
        if len(fields) < 3:
            continue

        try:
            altitude_km = float(fields[0])
            pressure_bar = float(fields[1])
            temperature_k = float(fields[2])
        except ValueError:
            continue

        rows.append((altitude_km, pressure_bar, temperature_k))

    return rows


def build_profile() -> list[tuple[float, float, float]]:
    lines = PIONEER_SOURCE.read_text(encoding="ascii").splitlines()

    lower = extract_numeric_rows(
        lines,
        "Lower Atmosphere State Properties:  Sounder Probe",
        "Altitudes are relative to the sounder probe landing site",
    )
    middle = extract_numeric_rows(
        lines,
        "State Properties of the Middle Atmosphere of Venus: Pioneer Venus Sounder Probe",
        "R0=6052.0 km",
    )

    # SP-8011 uses a 6050 km zero-altitude radius. Shift its altitudes onto
    # the project's 6052 km radius, then convert millibar to bar.
    upper = [
        (
            altitude_km
            + SP8011_REFERENCE_RADIUS_KM
            - PROJECT_REFERENCE_RADIUS_KM,
            pressure_millibar * 1.0e-3,
            temperature_k,
        )
        for altitude_km, temperature_k, pressure_millibar in SP8011_MODEL_I
    ]

    profile = sorted((*upper, *middle, *lower), reverse=True)

    if len(lower) != 66 or len(middle) != 30 or len(profile) != 120:
        raise RuntimeError(
            "Unexpected Venus source row count: "
            f"lower={len(lower)}, middle={len(middle)}, total={len(profile)}"
        )

    for previous, current in zip(profile, profile[1:]):
        if previous[0] <= current[0]:
            raise RuntimeError("Venus output altitudes are not strictly descending")
        if previous[1] <= 0.0 or current[1] <= 0.0:
            raise RuntimeError("Venus output contains a non-positive pressure")

    return profile


def main() -> None:
    profile = build_profile()

    with OUTPUT.open("w", encoding="ascii", newline="") as output_file:
        writer = csv.writer(output_file, lineterminator="\n")
        writer.writerow(("Altitude", "Pressure", "TempKelvin"))
        for altitude_km, pressure_bar, temperature_k in profile:
            writer.writerow(
                (
                    f"{altitude_km:.6g}",
                    f"{pressure_bar:.9e}",
                    f"{temperature_k:.6g}",
                )
            )


if __name__ == "__main__":
    main()
