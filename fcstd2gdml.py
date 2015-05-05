import argparse
import xml.etree.ElementTree as ET

import sys
sys.path.append('/usr/lib/freecad/lib/')
import FreeCAD

parser = argparse.ArgumentParser()
parser.add_argument('fcstd_input')
parser.add_argument('xml_output')

args = parser.parse_args()

FreeCAD.openDocument(args.fcstd_input)
fcstd = FreeCAD.activeDocument()

gdml = ET.Element('gdml', {
    'xmlns:xsi': "http://www.w3.org/2001/XMLSchema-instance",
    'xsi:noNamespaceSchemaLocation': "http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd"
})
define = ET.SubElement(gdml, 'define')
materials = ET.SubElement(gdml, 'materials')
solids = ET.SubElement(gdml, 'solids')
structure = ET.SubElement(gdml, 'structure')
setup = ET.SubElement(gdml, 'setup', {'name': 'Default', 'version': '1.0'})
ET.SubElement(setup, 'world', {'ref': 'worldLogical'})

booleans = {'Part::Cut': 'subtraction',
            'Part::Fuse': 'union',
            'Part::Common': 'intersection'}

placements = []
def addPlacement(physvol, placement, prefix=""):
    placements.append((physvol, placement, prefix))

bbox = FreeCAD.BoundBox()
worldLogical = ET.Element('volume', {'name': 'worldLogical'})
ET.SubElement(worldLogical, 'materialref', {'ref': 'G4_AIR'})
ET.SubElement(worldLogical, 'solidref', {'ref': 'world'})

# Calculate bounding box
for obj in fcstd.Objects:
    #print("{} + {} = ".format(bbox, obj.Shape.BoundBox))
    bbox.add(obj.Shape.BoundBox)
    #print(bbox)

# Fix placements
for obj in fcstd.Objects:
    if obj.TypeId == 'Part::Cylinder':
        # FreeCAD placement is center of bottom face
        # GDML position is center
        delta = FreeCAD.Vector(0, 0, obj.Height.Value / 2)
    elif obj.TypeId == 'Part::Box':
        # FreeCAD placement is a corner
        # GDML position is center
        delta = FreeCAD.Vector(obj.Length.Value / 2,
                               obj.Width.Value / 2,
                               obj.Height.Value / 2)
    else:
        continue
    rot = FreeCAD.Rotation(obj.Placement.Rotation)
    #rot.invert()
    obj.Placement.move(rot.multVec(delta))#.negative())

for obj in fcstd.Objects:
    if '!' in obj.Label:
        name, material = obj.Label.split('!')
    else:
        name = obj.Label
        material = None
    if obj.TypeId == 'Part::Cylinder':
        ET.SubElement(solids, 'tube', {
            'name': name, 'rmax': str(obj.Radius.Value), 
            'deltaphi': str(float(obj.Angle)), 'aunit': 'deg',
            'z': str(obj.Height.Value),
            'lunit': 'mm'})
    elif obj.TypeId == 'Part::Box':
        ET.SubElement(solids, 'box', {
            'name': name, 'x': str(obj.Length.Value/1), 
            'y': str(obj.Width.Value/1), 
            'z': str(obj.Height.Value/1),
            'lunit': 'mm'})
    elif obj.TypeId in booleans:
        solid = ET.SubElement(solids, booleans[obj.TypeId], { 'name': name })
        first = obj.OutList[0]
        ET.SubElement(solid, 'first', {'ref': first.Label.split('!')[0]})
        second = obj.OutList[1]
        ET.SubElement(solid, 'second', {'ref': second.Label.split('!')[0]})
        addPlacement(solid, second.Placement)
        addPlacement(solid, first.Placement, 'first')
    else:
        print "Unrecognized TypeId: {}".format(obj.TypeId)
        continue
    if material:
        # Create logical and physical volume
        logic = ET.SubElement(structure, 'volume', {'name': name+'Logical'})
        ET.SubElement(logic, 'materialref', {'ref': material})
        ET.SubElement(logic, 'solidref', {'ref': name})
        phys = ET.SubElement(worldLogical, 'physvol')
        ET.SubElement(phys, 'volumeref', {'ref': name+'Logical'})
        addPlacement(phys, obj.Placement)

ET.SubElement(solids, 'box', {
    'name': 'world', 'x': str(2*max(abs(bbox.XMin), abs(bbox.XMax))),
                     'y': str(2*max(abs(bbox.YMin), abs(bbox.YMax))),
                     'z': str(2*max(abs(bbox.ZMin), abs(bbox.ZMax))),
                     'lunit': 'mm'})
counter = 0
for physvol, placement, prefix in placements:
    angles = placement.Rotation.toEuler()
    pos = placement.Rotation.multVec(placement.Base)
    ET.SubElement(physvol, prefix+'position', {
        'unit': 'mm', 'name': 'anonymous{}'.format(counter),
        'x': str(pos[0]), 'y': str(pos[1]), 'z': str(pos[2])})
    counter += 1
    ET.SubElement(physvol, prefix+'rotation', {
        'unit': 'deg', 'name': 'anonymous{}'.format(counter),
        'x': str(-angles[2]), 'y': str(-angles[1]), 'z': str(-angles[0])})
    counter += 1

structure.append(worldLogical)

ET.ElementTree(gdml).write(args.xml_output, 'utf-8', True)
