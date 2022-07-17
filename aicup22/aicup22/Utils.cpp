#include <Utils.hpp>
#include <limits>
#include <cmath>

std::pair<std::vector<model::Unit>, std::vector<model::Unit>> getUnits(const model::Game &game)
{
    std::vector<model::Unit> other_units;
    std::vector<model::Unit> my_units;
    for (auto &unit : game.units)
    {
        if (unit.playerId == game.myId)
        {
            my_units.push_back(unit);
            continue;
        }
        
        other_units.push_back(unit);
    }

    return std::pair(my_units, other_units);
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

model::Vec2 getCurrentZoneCenter(const model::Game &game, const model::Unit& unit)
{
    return vecDiff( game.zone.currentCenter, unit.position );
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

    debugInterface->addPlacedText(pos, text, {0.0, 0.0}, 0.4, {0.2, 0.2, 0.2, 1.0});
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

std::vector<model::Obstacle> removeObstaclesInsideCircle(const std::vector<model::Obstacle>& obstacles, const model::Vec2& circle_pos, double rad)
{
    std::vector<model::Obstacle> result;
    for (auto obs : obstacles)
    {
        if (!isVecInsideCircle(circle_pos, rad, obs.position))
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

std::optional<model::Loot> closestLoot(const model::Vec2& point, const std::vector<model::Loot>& loot)
{
   double minDistance = std::numeric_limits<double>::max(); 
   std::optional<model::Loot> closest_loot;
   for (auto l : loot){
       double distance = countRange(l.position, point);
       if (distance < minDistance)
       {
          minDistance = distance;
          closest_loot = l;
       }
   }

   return closest_loot;
}

void highlightObstacle(const model::Obstacle& obstacle, DebugInterface *debugInterface)
{

    if (debugInterface == nullptr)
        return;

    debugInterface->addRing(obstacle.position, obstacle.radius + 1.0, 0.5, debugging::Color(1.0, 0.2, 0.0, 0.7));
}

void highlightSounds(const std::vector<model::Sound>& sounds, DebugInterface *debugInterface)
{
    if (debugInterface == nullptr)
        return;

    for (auto s : sounds)
    {
        debugInterface->addRing(s.position, 2.0, 0.5, debugging::Color(0.0, 0.5, 0.0, 0.2));
    }
}

void highlightLoot(const std::vector<model::Loot>& loot, DebugInterface *debugInterface, int type)
{
    if (debugInterface == nullptr)
        return;

    for (const auto l : loot)
    {
        if (type == 1)
            debugInterface->addRing(l.position, 0.7, 0.1, debugging::Color(0.2, 0.2, 1.0, 0.4));
        else if (type == 2)
            debugInterface->addRing(l.position, 0.7, 0.1, debugging::Color(0.2, 1.0, 0.2, 0.4));
        else
            debugInterface->addRing(l.position, 0.7, 0.1, debugging::Color(1.0, 0.2, 0.2, 0.4));

    }
}

bool isAimInObs(const model::Vec2& pos1, const model::Vec2& pos2, const std::vector<model::Obstacle>& obstacles)
{
    for (auto obs : obstacles)
    {
        if (isAimInObs(pos1, pos2, obs)) 
            return true;
    }

    return false;
}

bool isAimInObs(const model::Vec2& pos1, const model::Vec2& pos2, const model::Obstacle& obs)
{
    auto a = 1 / (pos2.x - pos1.x);
    auto b = - 1 / (pos2.y - pos1.y);
    auto c = - pos1.x / (pos2.x - pos1.x) + pos1.y / (pos2.y - pos1.y); 
    auto d = ( a * obs.position.x + b * obs.position.y + c ) / sqrt(a*a + b*b);
    d = d < 0 ? -d : d;
    return d < obs.radius - 0.05;
}

void drawLine(const model::Vec2& pos1, const model::Vec2& pos2, DebugInterface *debugInterface)
{
    if (debugInterface == nullptr)
        return;

    auto vertices = std::vector<model::Vec2>();
    vertices.push_back(pos1);
    vertices.push_back(pos2);
    debugInterface->addPolyLine(vertices, 0.2, debugging::Color(0.0, 0.6, 0.0, 1.0)); 
}
