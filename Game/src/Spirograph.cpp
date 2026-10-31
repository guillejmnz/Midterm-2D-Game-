#include "Spirograph.h"

#include <algorithm> // std::min
#include <cmath>
#include <numeric>   // std::lcm

Vector2 HypotrochoidPoint(const Hypotrochoid& h, float theta)
{
    const float diff  = static_cast<float>(h.R - h.r);
    const float ratio = diff / static_cast<float>(h.r);

    // x = (R - r) cos(theta) + d cos(((R - r) / r) theta)
    // y = (R - r) sin(theta) - d sin(((R - r) / r) theta)
    return Vector2{
        diff * std::cos(theta) + h.d * std::cos(ratio * theta),
        diff * std::sin(theta) - h.d * std::sin(ratio * theta)
    };
}

float HypotrochoidPeriod(const Hypotrochoid& h)
{
    return 2.0f * PI * static_cast<float>(std::lcm(h.r, h.R)) / static_cast<float>(h.R);
}

Vector2 HypotrochoidScreenPoint(const Hypotrochoid& h, float theta, Vector2 center)
{
    Vector2 p = HypotrochoidPoint(h, theta);
    return Vector2{ center.x + p.x, center.y + p.y };
}

// ----------------------------------------------------------------------
// Spirograph
// ----------------------------------------------------------------------

Spirograph::Spirograph(const Hypotrochoid& curve, Vector2 center, float step, int pointsPerFrame)
    : m_Curve(curve)
    , m_Center(center)
    , m_Step(step)
    , m_PointsPerFrame(pointsPerFrame)
    , m_Period(HypotrochoidPeriod(curve))
    , m_TotalPoints(static_cast<int>(std::ceil(m_Period / step)) + 1)   // index 0 through the period
    , m_PointIndex(0)
    , m_HoleDistances{}
    , m_ActiveHole(HoleCount - 1)
{
    // One allocation for the whole curve: push_back never has to grow the
    // vector, so the points are never copied to a new block mid-animation
    m_Points.reserve(static_cast<std::size_t>(m_TotalPoints));

    // Every hole must sit inside the gear (distance <= r). The gear never leaves
    // the outer circle, so the pattern can never leave it either.
    const float outermost = std::min(curve.d, static_cast<float>(curve.r));
    for (int i = 0; i < HoleCount; ++i)
    {
        m_HoleDistances[i] = outermost * static_cast<float>(i + 1) / static_cast<float>(HoleCount);
    }

    // Start with the pen in the outermost hole
    m_Curve.d = m_HoleDistances[m_ActiveHole];
}

void Spirograph::Update()
{
    // Add new points until the curve closes
    for (int i = 0; i < m_PointsPerFrame && !IsComplete(); ++i, ++m_PointIndex)
    {
        // theta from an integer index avoids float drift from repeated += step
        float theta = static_cast<float>(m_PointIndex) * m_Step;
        if (theta > m_Period) theta = m_Period;
        m_Points.push_back(HypotrochoidScreenPoint(m_Curve, theta, m_Center));
    }
}

void Spirograph::Draw(Color color) const
{
    if (m_Points.size() >= 2)
    {
        DrawLineStrip(m_Points.data(), static_cast<int>(m_Points.size()), color);
    }
}

void Spirograph::DrawOuterCircle(Color color) const
{
    DrawCircleLinesV(m_Center, static_cast<float>(m_Curve.R), color);
}

void Spirograph::DrawPen(float radius, Color color) const
{
    // The pen sits on the last traced point, or at the start before the first Update
    const Vector2 pen = m_Points.empty()
        ? HypotrochoidScreenPoint(m_Curve, 0.0f, m_Center)
        : m_Points.back();

    DrawCircleV(pen, radius, color);
}

void Spirograph::DrawGear(Color gearColor, Color holeColor) const
{
    const float theta = CurrentTheta();
    const Vector2 gear = GearCenter(theta);
    const float radius = static_cast<float>(m_Curve.r);

    DrawCircleV(gear, radius, Fade(gearColor, 0.25f));
    DrawCircleLinesV(gear, radius, gearColor);

    for (int i = 0; i < HoleCount; ++i)
    {
        DrawCircleLinesV(HolePosition(i, theta), 4.0f, holeColor);
    }
}

bool Spirograph::IsComplete() const
{
    return m_PointIndex >= m_TotalPoints;
}

void Spirograph::SetActiveHole(int hole)
{
    if (hole < 0 || hole >= HoleCount || hole == m_ActiveHole)
    {
        return;
    }

    m_ActiveHole = hole;
    m_Curve.d = m_HoleDistances[hole];

    // Restart the trace. clear() keeps the reserved capacity, and the point count
    // does not depend on d, so the new trace reuses the same allocation.
    m_Points.clear();
    m_PointIndex = 0;
}

int Spirograph::GetActiveHole() const
{
    return m_ActiveHole;
}

float Spirograph::CurrentTheta() const
{
    if (m_PointIndex == 0)
    {
        return 0.0f;
    }

    const float theta = static_cast<float>(m_PointIndex - 1) * m_Step;
    return theta > m_Period ? m_Period : theta;
}

Vector2 Spirograph::GearCenter(float theta) const
{
    // The gear touches the outer circle from inside, so its center
    // orbits the middle at distance R - r
    const float orbit = static_cast<float>(m_Curve.R - m_Curve.r);
    return Vector2{
        m_Center.x + orbit * std::cos(theta),
        m_Center.y + orbit * std::sin(theta)
    };
}

Vector2 Spirograph::HolePosition(int hole, float theta) const
{
    // Rolling without slipping turns the gear the opposite way by ((R - r) / r) * theta.
    // This is the same term as in the hypotrochoid formula, so the active hole
    // lands exactly on the traced curve.
    const float spin = static_cast<float>(m_Curve.R - m_Curve.r) / static_cast<float>(m_Curve.r) * theta;
    const float distance = m_HoleDistances[hole];
    const Vector2 gear = GearCenter(theta);

    return Vector2{
        gear.x + distance * std::cos(spin),
        gear.y - distance * std::sin(spin)
    };
}
