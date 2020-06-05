// All dimensions in millimeters

// Invariants
BUTTON_SHAFT_DIAMETER = 29.35;
BUTTON_OUTER_DIAMETER = 33.75;
BUTTON_FACE_THICKNESS = 5.00; // Approximate
BUTTON_SHAFT_LENGTH = 19.50;

SWITCH_SHAFT_DIAMETER = 11.58;
SWITCH_SHAFT_LENGTH = 11.30; // Total length, washer is inside the shaft
SWITCH_WASHER_DIAMETER = 17.45;
SWITCH_WASHER_THICKNESS = 5.05;

SWITCH_BOX_LENGTH = 24.50;
SWITCH_BOX_WIDTH = 14.05;
SWITCH_BOX_DEPTH = 13.95;

// Variables
ENCLOSURE_BACK_HEIGHT = 48.00;
FRAME_THICKNESS = 4.00;
COMPONENT_MARGIN = 4.00;
PANEL_MARGIN_HORIZONTAL = 8.00;
PANEL_MARGIN_VERTICAL = 10.00;
PANEL_SLOPE_DEGREES = 15.0;

// Derived

PANEL_WIDTH = BUTTON_OUTER_DIAMETER + (PANEL_MARGIN_VERTICAL * 2);
PANEL_LENGTH = (PANEL_MARGIN_HORIZONTAL * 2) + (BUTTON_OUTER_DIAMETER * 2) + SWITCH_WASHER_DIAMETER + (COMPONENT_MARGIN * 2);

PANEL_DISTANCE_HORIZONTAL = PANEL_WIDTH * cos(PANEL_SLOPE_DEGREES);
PANEL_DISTANCE_VERTICAL = PANEL_WIDTH * sin(PANEL_SLOPE_DEGREES);

PANEl_BACK_HEIGHT = ENCLOSURE_BACK_HEIGHT;
PANEL_BACK_POSITION_HORIZONTAL = PANEL_DISTANCE_HORIZONTAL - FRAME_THICKNESS;

PANEL_FRONT_HEIGHT = PANEl_BACK_HEIGHT - PANEL_DISTANCE_VERTICAL;

COMPONENT_CENTER_HORIZONTAL = (PANEL_WIDTH / 2) * cos(PANEL_SLOPE_DEGREES);
COMPONENT_CENTER_VERTICAL = PANEL_FRONT_HEIGHT + ((PANEL_WIDTH / 2) * sin(PANEL_SLOPE_DEGREES));

BUTTON_A_CENTER_X = PANEL_MARGIN_HORIZONTAL + (BUTTON_OUTER_DIAMETER / 2);
BUTTON_B_CENTER_X = PANEL_MARGIN_HORIZONTAL + BUTTON_OUTER_DIAMETER + COMPONENT_MARGIN + (BUTTON_OUTER_DIAMETER / 2);

SWITCH_CENTER_X = PANEL_LENGTH - (PANEL_MARGIN_HORIZONTAL + (SWITCH_WASHER_DIAMETER / 2));
SWITCH_BOX_OFFSET_Z = -1 * ((SWITCH_BOX_DEPTH / 2) + FRAME_THICKNESS);

// Models

module assembly() {
  enclosure();

  // button();

  // switch();
}

module button() {
  color("blue") {
    union() {
      // Shaft
      translate([0.0, 0.0, -BUTTON_SHAFT_LENGTH]) {
        cylinder(h=BUTTON_SHAFT_LENGTH, r=(BUTTON_SHAFT_DIAMETER / 2), $fn=100);
      }

      // Top
      cylinder(h=BUTTON_FACE_THICKNESS, r=(BUTTON_OUTER_DIAMETER / 2), $fn=100);
    }
  }
}

module switch() {
  color("green") {
    union() {
      // Top
      cylinder(h=SWITCH_WASHER_THICKNESS, r=(SWITCH_WASHER_DIAMETER / 2), $fn=100);

      // Shaft
      translate([0.0, 0.0, -FRAME_THICKNESS]) {
        cylinder(h=SWITCH_SHAFT_LENGTH, r=(SWITCH_SHAFT_DIAMETER / 2), $fn=100);
      }

      // Box
      translate([0.0, 0.0, SWITCH_BOX_OFFSET_Z]) {
        cube([SWITCH_BOX_WIDTH, SWITCH_BOX_LENGTH, SWITCH_BOX_DEPTH], center=true);
      }
    }
  }
}

module enclosure() {
  difference() {
    // Enclosure itself
    rotate([90.0, 0.0, 90.0]) {
      linear_extrude(PANEL_LENGTH, convexity=10) {
        side_sketch();
      }
    }

    // Button A
    translate([BUTTON_A_CENTER_X, COMPONENT_CENTER_HORIZONTAL, COMPONENT_CENTER_VERTICAL]) {
      rotate([PANEL_SLOPE_DEGREES, 0.0, 0.0]) {
        button();
      }
    }

    // Button B
    translate([BUTTON_B_CENTER_X, COMPONENT_CENTER_HORIZONTAL, COMPONENT_CENTER_VERTICAL]) {
      rotate([PANEL_SLOPE_DEGREES, 0.0, 0.0]) {
        button();
      }
    }

    //Switch
    translate([SWITCH_CENTER_X, COMPONENT_CENTER_HORIZONTAL, COMPONENT_CENTER_VERTICAL]) {
      rotate([PANEL_SLOPE_DEGREES, 0.0, 0.0]) {
        switch();
      }
    }
  }
}

module side_sketch() {
  difference() {
    union() {
      // Front
      square([FRAME_THICKNESS, PANEL_FRONT_HEIGHT]);

      // Back
      translate([PANEL_BACK_POSITION_HORIZONTAL, 0.0, 0.0]) {
        square([FRAME_THICKNESS, PANEl_BACK_HEIGHT]);
      }

      // Panel (diagonal across the top)
      translate([0.0, PANEL_FRONT_HEIGHT, 0.0]) {
        rotate([0.0, 0.0, (PANEL_SLOPE_DEGREES - 90.0)]) {
          square([FRAME_THICKNESS, PANEL_WIDTH]);
        }
      }

      // Bottom
      square([PANEL_DISTANCE_HORIZONTAL, FRAME_THICKNESS]);
    }

    // Mask the back edge
    translate([PANEL_BACK_POSITION_HORIZONTAL + FRAME_THICKNESS, 0.0, 0.0]) {
      color("black") square([FRAME_THICKNESS, PANEl_BACK_HEIGHT]);
    }

    // Mask the panel edge
    translate([0.0, PANEL_FRONT_HEIGHT, 0.0]) {
      rotate([0.0, 0.0, (PANEL_SLOPE_DEGREES - 90.0)]) {
        translate([-FRAME_THICKNESS, 0.0, 0.0]) {
          color("black") square([FRAME_THICKNESS, PANEL_WIDTH]);
        }
      }
    }
  }
}

assembly();
