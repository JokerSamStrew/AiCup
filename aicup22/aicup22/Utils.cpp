#include <Utils.hpp>
#include <limits>
#include <cmath>

std::pair<std::optional<model::Unit>, std::vector<model::Unit>> getUnits(const model::Game &game)
{
    std::vector<model::Unit> other_units;
    std::optional<model::Unit> my_unit;
    for (auto &unit : game.units)
    {
        if (unit.playerId == game.myId)
        {
            my_unit = unit;
            continue;
        }
        
        other_units.push_back(unit);
    }

    return std::pair(my_unit, other_units);
}

double countRange(const model::Vec2& a, const model::Vec2& b)
{
   return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

std::optional<model::Unit> closestUnit(const model::Vec2& point, const std::vector<model::Unit>& units)
{
   double minDistance = std::numeric_limits<double>::max(); 
   std::optional<model::Unit> closestUnit;
   for (auto unit : units){
       double distance = countRange(unit.position, point);
       if (distance < minDistance)
       {
          minDistance = distance;
          closestUnit = unit;
       }
   }

   return closestUnit;
}

void highlightUnits(std::vector<model::Unit>& units, DebugInterface *debugInterface)
{
    if (debugInterface == nullptr)
        return;

    for (const auto &unit : units)
    {
        debugInterface->addRing(unit.position, 2, 0.5, debugging::Color(0.0, 0.5, 0.0, 0.5));
    }
}

void drawDirectionArc(const model::Unit& unit, double lineSize, DebugInterface *debugInterface)
{
   if (debugInterface == nullptr)
       return;

   auto end_position = model::Vec2(
           unit.position.x + lineSize * unit.direction.x,
           unit.position.y + lineSize * unit.direction.y );
   debugInterface->addPolyLine({unit.position, end_position}, 0.5, debugging::Color(0.0, 0.0, 1.0, 0.5));
}

model::Vec2 getNextZoneCenter(const model::Game &game, const model::Unit& unit)
{
    return vecDiff( game.zone.nextCenter, unit.position );
}

model::Vec2 vecDiff(const model::Vec2& a, const model::Vec2& b)
{
    return {a.x - b.x, a.y - b.y};
}

model::Vec2 vecSum(const model::Vec2& a, const model::Vec2& b)
{
    return {a.x + b.x, a.y + b.y};
}

void drawText(const model::Vec2& pos, const std::string& text, DebugInterface *debugInterface)
{
    if (debugInterface == 0)
        return;

    debugInterface->addPlacedText(pos, text, {2.0, 2.0}, 3.0, {0.0, 0.0, 0.0, 1.0});
}

void highlightObstacles(const std::vector<model::Obstacle>& obstacles, DebugInterface *debugInterface)
{

    if (debugInterface == nullptr)
        return;

    for (const auto &obs : obstacles)
    {
        debugInterface->addRing(obs.position, obs.radius + 1.0, 0.5, debugging::Color(1.0, 0.6, 0.0, 0.5));
    }
}

bool isVecInsideCircle(const model::Vec2& circle, double rad, const model::Vec2& pos)
{
    // Compare radius of circle with distance
    // of its center from given point
    if ((pos.x - circle.x) * (pos.x - circle.x) +
        (pos.y - circle.y) * (pos.y - circle.y) <= rad * rad)
        return true;
    else
        return false;
}

std::vector<model::Obstacle> getObstaclesInsideCircle(const std::vector<model::Obstacle>& obstacles, const model::Vec2& circle_pos, double rad)
{
    std::vector<model::Obstacle> result;
    for (auto obs : obstacles)
    {
        if (isVecInsideCircle(circle_pos, rad, obs.position))
            result.push_back(obs);
    }

    return result;
}


std::optional<model::Obstacle> closestObstacle(const model::Vec2& point, const std::vector<model::Obstacle>& obstacles)
{
   double minDistance = std::numeric_limits<double>::max(); 
   std::optional<model::Obstacle> closest_obs;
   for (auto obs : obstacles){
       double distance = countRange(obs.position, point);
       if (distance < minDistance)
       {
          minDistance = distance;
          closest_obs = obs;
       }
   }

   return closest_obs;
}


void highlightObstacle(const model::Obstacle& obstacle, DebugInterface *debugInterface)
{

    if (debugInterface == nullptr)
        return;

    debugInterface->addRing(obstacle.position, obstacle.radius + 1.0, 0.5, debugging::Color(1.0, 0.2, 0.0, 0.7));
}

void highlightLoot(const std::vector<model::Loot>& loot, DebugInterface *debugInterface)
{
    if (debugInterface == nullptr)
        return;

    for (const auto l : loot)
    {
        debugInterface->addRing(l.position, 0.7, 0.3, debugging::Color(0.2, 0.2, 1.0, 0.4));
    }
}
