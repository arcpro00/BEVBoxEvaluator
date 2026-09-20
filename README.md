# BEVBoxEvaluator

A lightweight C++ command-line tool for evaluating overlap between 3D bounding boxes, with applications in autonomous-driving perception.

The evaluator uses a **2.5D geometric representation**, modeling objects as upright rectangular cuboids with yaw-only rotation. This simplifies 3D overlap calculations into bird's-eye-view (BEV) polygon intersection and vertical height overlap.

## Features

- **2.5D box representation:** Represents 3D boxes using a top-down view and height.
- **Yaw rotation:** Handles boxes rotated around the vertical axis without complex 3D calculations.
- **Exact BEV intersection:** Computes intersection areas using polygon clipping.
- **3D IoU evaluation:** Combines horizontal intersection area with vertical overlap.
- **Batch processing:** Evaluates multiple bounding-box pairs from a text file.
- **Evaluation statistics:** Reports per-pair results and aggregate IoU statistics.
- **Pair matching:** Matches boxes using their `pair_id`, even when corresponding rows are not adjacent.

## Requirements

- A C++17-compatible compiler
- CMake 3.16 or newer

## Building the Project

Clone the repository and navigate to the project directory.

Configure the CMake build in Release mode:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

Compile the executable:

```bash
cmake --build build
```

On Linux and macOS using a single-configuration CMake generator, the executable will typically be located at `build/box_eval_app`.

## Running the Evaluator

Run the executable with the path to an input data file:

```bash
./build/box_eval_app noisy_test_data.txt
```

You can also provide a relative or absolute path to another data file:

```bash
./build/box_eval_app data/sample_boxes.txt
```

The program reads the file, groups bounding boxes by pair ID, evaluates each pair, and prints the results to the terminal.

### Example Output

```text
Pair ID | Area Overlap | Height Overlap | 3D IoU
------------------------------------------------
0       | 12.4512      | 1.8000         | 0.7341
1       | 8.1234       | 1.5000         | 0.5123
2       | 0.0000       | 0.0000         | 0.0000

Pairs evaluated:     3
Mean 3D IoU:         0.4155
Minimum 3D IoU:      0.0000
Maximum 3D IoU:      0.7341
Zero-overlap pairs:  1
```

## Input Data Format

The evaluator accepts a whitespace-separated text file containing one bounding box per row.

The first line must be a header:

```text
pair_id x y z length width height yaw
```

Each subsequent line contains eight values in the same order.

### Column Specifications

| Column | Type | Description |
|---|---|---|
| `pair_id` | Integer | Identifier associating two boxes with the same evaluation pair |
| `x` | Float | X-coordinate of the box center |
| `y` | Float | Y-coordinate of the box center |
| `z` | Float | Z-coordinate of the box's bottom face |
| `length` | Float | Box dimension along its local X-axis |
| `width` | Float | Box dimension along its local Y-axis |
| `height` | Float | Vertical box dimension |
| `yaw` | Float | Rotation around the Z-axis, in radians |

### Generating Example Data

The repository includes `generate_test_data.py`, a Python script that generates sample bounding-box pairs for testing the evaluator.

Each pair contains a randomly generated 3D bounding box and a second box with modified coordinates, dimensions, and yaw to simulate differences between predicted and reference bounding boxes.

Run the script to generate `noisy_test_data.txt`:

```bash
python3 generate_test_data.py
```

Then build and run the evaluator:

```bash
./build/box_eval_app noisy_test_data.txt
```

By default, the script generates 100 pairs (200 bounding-box records).

### Example Input File

```text
pair_id x y z length width height yaw
0 12.5 8.2 4.1 6.0 3.0 2.0 0.5
1 5.0 4.0 0.0 4.0 2.0 1.5 0.0
0 12.8 8.0 4.2 6.2 3.1 2.1 0.6
1 5.5 4.2 0.0 4.0 2.0 1.5 0.1
```

### Input Rules

- Each row represents exactly one 3D bounding box.
- Each `pair_id` must occur **exactly twice**.
- Rows belonging to the same pair do not need to be adjacent.
- Pair IDs do not need to be consecutive.
- Values must be separated by whitespace (spaces or tabs).
- The first line is treated as the header and skipped during parsing.
- Length, width, and height should be positive.
- Yaw values are specified in radians.
- The order of evaluated pairs is not guaranteed.

The two boxes associated with a given `pair_id` are evaluated against each other.

## Evaluation Method

### BEV Intersection Area

Each 3D bounding box is projected onto the XY plane as a rotated rectangle. Its four corners are computed using the box's center, dimensions, and yaw.

The intersection polygon of the two rectangles is calculated using the Sutherland–Hodgman polygon-clipping algorithm. Its area is then calculated using the shoelace formula.

### Vertical Overlap

The overlap along the Z-axis is:

\[
h_{\text{overlap}} = \max(0, \min(z_1+h_1,z_2+h_2)-\max(z_1,z_2))
\]

### 3D Intersection-over-Union

The intersection volume is:

\[
V_{\text{intersection}} = A_{\text{overlap}} \times h_{\text{overlap}}
\]

The 3D IoU is:

\[
\text{IoU}_{3D} = \frac{V_{\text{intersection}}}{V_1+V_2-V_{\text{intersection}}}
\]

The resulting IoU ranges from 0 (no overlap) to 1 (identical boxes).

## Limitations

- Assumes upright bounding boxes with yaw-only rotation; pitch and roll are not supported.
- Assumes a common vertical axis and locally flat ground-plane approximation.
- Computes exact overlap for the represented upright cuboids, not for arbitrary rotated 3D objects.
- Evaluates pre-associated box pairs.