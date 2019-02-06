# geanTipi
Geant application that imports a FreeCAD model and records deposited energy

## Usage

### Converting FreeCAD models to GDML

```
python fcstd2gdml.py input.fcstd output.gdml
```

### Running

Create a command file (look at `exampleB1.in` for guidance) and run it using

```
./exampleB1 your_commands.in
```

It should generate a number of files named `RunNNNNNN` with the following columns:
* Energy
* x
* y
* z
* Weight (1 unless you're using biasing)
