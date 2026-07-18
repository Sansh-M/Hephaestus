# Sourced atmospheric profiles

This folder contains authoritative source tables in `raw/` and project-ready CSVs in `profiles/` for every body currently marked `hasSignificantAtmosphere: true` in `Universe.json`, other than Venus (which already has files).

## Project-ready CSV schema

Every file in `profiles/` uses the same three-column schema and descending altitude order:

```text
Altitude,Pressure,TempKelvin
```

- `Altitude`: km. For gas giants, zero is the 1-bar reference level unless the source states otherwise.
- `Pressure`: bar.
- `TempKelvin`: K.

Raw archive values are retained unchanged in `raw/`. Only column selection, unit conversion, ordering, and the explicitly documented Neptune altitude reconstruction were applied to the project-ready files.

All project-ready CSVs have exactly three values per row and strictly descending altitude. Pressure increases with descent in every profile except for four reversals retained from the recovered Viking 1 source (three reconstruction irregularities/possible source typos and one overlap between low-altitude profile phases); no smoothing was applied.

## Mission and source map

| Target | Project-ready file | Mission / reference | Instrument / product | Official source | Important limitation |
|---|---|---|---|---|---|
| Earth | `EARTH_US_STANDARD_ATMOSPHERE_1976.csv` | U.S. Standard Atmosphere 1976; not a space mission | COESA reference atmosphere | [NASA PDS Atmospheres Node table](https://atmos.nmsu.edu/planetary_datasets/earth_temppres.html); [NASA-TM-X-74335](https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770009539.pdf) | Idealized standard atmosphere, 0-86 km, not a time- or location-specific observation. |
| Mars | `MARS_VIKING_1_ENTRY.csv` | Viking 1 Lander, 1976 | Entry-vehicle accelerometer atmospheric reconstruction | [NASA PDS Atmospheres Node entry-profile page](https://atmos.nmsu.edu/data_and_services/atmospheres_data/MARS/viking/entry_profiles.html); [recovered Viking 1 table](https://atmos.nmsu.edu/data_and_services/atmospheres_data/MARS/viking/logs/VL1_entry_profile.txt) | The PDS page says these profiles were recovered rather than originally archived. Values below about 5 km include extrapolated and descent-phase rows identified in the raw file. |
| Mars | `MARS_VIKING_2_ENTRY.csv` | Viking 2 Lander, 1976 | Entry-vehicle accelerometer atmospheric reconstruction | [NASA PDS Atmospheres Node entry-profile page](https://atmos.nmsu.edu/data_and_services/atmospheres_data/MARS/viking/entry_profiles.html); [recovered Viking 2 table](https://atmos.nmsu.edu/data_and_services/atmospheres_data/MARS/viking/logs/VL2_entry_profile.txt) | Profile ends near 26 km and does not reach the surface. |
| Jupiter | `JUPITER_GALILEO_ASI.csv` | Galileo Probe entry, 1995 | Atmospheric Structure Instrument (ASI), data set `GP-J-ASI-3-ENTRY-V1.0` | [NASA PDS ASI page](https://pds-atmospheres.nmsu.edu/data_and_services/atmospheres_data/Galileo/asi.html); [PDS data volume](https://pds-atmospheres.nmsu.edu/PDS/data/gp_0001/); [DOI](https://doi.org/10.17189/tfsa-pb91) | One entry location in a Jovian hot spot; not a global mean profile. Upper and lower ASI tables were joined without filling the approximately 5 km phase gap. |
| Saturn | `SATURN_VOYAGER_2_RSS_INGRESS.csv` | Voyager 2 Saturn encounter, 1981 | Radio Science Subsystem (RSS) ingress occultation, publication-derived table | [NASA PDS Atmospheres Node table page](https://atmos.nmsu.edu/planetary_datasets/saturn_temppres.html); [ASCII source](https://atmos.nmsu.edu/planetary_datasets/saturntemppres.txt) | Remote-sensing retrieval rather than an entry probe; covers about 0.2-1298 mbar and uses the 1-bar level as zero altitude. |
| Titan | `TITAN_HUYGENS_HASI.csv` | Cassini-Huygens / Huygens Probe descent, 2005 | Huygens Atmospheric Structure Instrument (HASI), data set `HP-SSA-HASI-2-3-4-MISSION-V1.1` | [NASA PDS HASI page](https://pds-atmospheres.nmsu.edu/data_and_services/atmospheres_data/Huygens/HASI.html); [PDS profile directory](https://atmos.nmsu.edu/PDS/data/PDS4/Huygens/hphasi_bundle/DATA/PROFILES/) | Entry and descent products are joined without interpolating the approximately 10 km phase gap. The downloaded v1.1 products incorporate the temperature/profile corrections described by the archive errata. |
| Uranus | `URANUS_VOYAGER_2_RSS_INGRESS.csv` | Voyager 2 Uranus encounter, 1986 | RSS ingress occultation, nominal atmosphere model | [NASA PDS Atmospheres Node table page](https://atmos.nmsu.edu/planetary_datasets/uranus_temppres.html); [ASCII source](https://atmos.nmsu.edu/planetary_datasets/uranustemppres.txt) | Remote-sensing retrieval, not in situ; assumes a helium-to-hydrogen abundance ratio of 15/85 and uses the 1-bar level as zero altitude. |
| Neptune | `NEPTUNE_VOYAGER_2_RSS_DERIVED_ALTITUDE.csv` | Voyager 2 Neptune encounter, 1989 | RSS occultation temperature-pressure profile | [NASA PDS Atmospheres Node table page](https://atmos.nmsu.edu/planetary_datasets/neptune_temppres.html); [ASCII source](https://atmos.nmsu.edu/planetary_datasets/neptunetempprestable.txt) | The NASA table has pressure and temperature but no altitude. Altitude was reconstructed as described below and is therefore processed, not a directly archived column. |

## Neptune altitude reconstruction

The source table was anchored at `0 km = 1000 mbar`. Adjacent pressure levels were integrated using hydrostatic balance and the ideal-gas law:

```text
delta_z = -(R * mean_temperature / (molar_mass * gravity)) * delta_ln_pressure
```

Constants: `R = 8.31446261815324 J mol^-1 K^-1`, molar mass `0.00253 kg mol^-1`, and gravity `11.15 m s^-2`. This produces a roughly 40 km separation between the 1-bar and 100-mbar levels, consistent with the Voyager 2 occultation result reported by Lindal.

## Bodies intentionally excluded

No profiles were added for Mercury, Io, Europa, Ganymede, Callisto, Enceladus, Titania, Oberon, or Triton. They are marked as negligible/exosphere/no-atmosphere cases in `Universe.json` and do not warrant the fluid-atmosphere drag tables requested here. The Sun is also excluded because the project does not model it as an atmospheric drag body.

## Existing Venus-file audit

The existing Venus files do not currently share a reliable three-column schema:

- `0VENUS1LOW2NULL.csv` appears to contain altitude, temperature, and **density**, while the third column is labeled `Pressure`. Its first value `0.1964` matches the density column in the NASA table; the corresponding pressure is `0.0925 bar`.
- `0VENUS1LOW2DAY.csv` has four data values under three headers. Its third value also matches density rather than pressure.
- `0VENUS1LOW2NIGHT.csv` begins with the values from the Pioneer Venus **North Probe** table, not the Night Probe table.

The new files deliberately use the verified canonical schema above. Existing Venus files were not changed as part of this sourcing pass.
