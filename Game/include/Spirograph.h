#pragma once

#include <raylib.h>
#include <array>
#include <vector>

struct Hypotrochoid
{
    int   R;   // fixed circle radius
    int   r;   // rolling circle radius
    float d;   // pen distance from rolling circle center
};

// Point on the curve at angle theta, relative to the origin (0, 0)
Vector2 HypotrochoidPoint(const Hypotrochoid& h, float theta);

// Theta value at which the curve closes: 2 * PI * LCM(r, R) / R
float HypotrochoidPeriod(const Hypotrochoid& h);

// Same point, shifted to a screen-space center
Vector2 HypotrochoidScreenPoint(const Hypotrochoid& h, float theta, Vector2 center);

// Draws one hypotrochoid over time.
// Update() advances the pen a few points per frame, Draw() renders the path so far.
// The rolling circle (gear) has HoleCount holes on one line through its center.
// curve.d is the distance of the outermost hole; the others are evenly spaced inward.
class Spirograph
{
public:
    static constexpr int HoleCount = 4;

    Spirograph(const Hypotrochoid& curve, Vector2 center, float step, int pointsPerFrame);

    // Copying would silently duplicate the whole point buffer, so it is disabled.
    // Moving hands the buffer over without copying; noexcept lets containers
    // and the compiler pick the move instead of falling back to a copy.
    Spirograph(const Spirograph&) = delete;
    Spirograph& operator=(const Spirograph&) = delete;
    Spirograph(Spirograph&&) noexcept = default;
    Spirograph& operator=(Spirograph&&) noexcept = default;
    ~Spirograph() = default;   // std::vector releases the points

    void Update();
    void Draw(Color color) const;
    void DrawOuterCircle(Color color) const;        // fixed circle of radius R
    void DrawPen(float radius, Color color) const;  // current pen position
    void DrawGear(Color gearColor, Color holeColor) const;  // rolling circle and its holes
    bool IsComplete() const;

    // Moves the pen to another hole (0 = closest to the gear center) and restarts the trace
    void SetActiveHole(int hole);
    int GetActiveHole() const;

private:
    Hypotrochoid m_Curve;
    Vector2 m_Center;
    float m_Step;               // theta increment per point
    int m_PointsPerFrame;       // drawing speed
    float m_Period;             // theta at which the curve closes
    int m_TotalPoints;          // points needed to close the curve
    int m_PointIndex;           // next point to trace; theta = m_PointIndex * m_Step

    std::vector<Vector2> m_Points;   // points traced so far, capacity reserved up front

    std::array<float, HoleCount> m_HoleDistances;   // distance of each hole from the gear center
    int m_ActiveHole;                               // hole the pen is in

    float CurrentTheta() const;                         // angle of the last traced point
    Vector2 GearCenter(float theta) const;              // center of the rolling circle
    Vector2 HolePosition(int hole, float theta) const;  // a hole, turned with the gear
};
