cmake --build ../aicup22/out
../app/aicup22 --antialias false --config ../aicup22/tests/app_configs/auto_run_config.json &
aicup22=$!
sleep 2
../aicup22/out/ai_cup_22 &
ai_cup_22=$!
wait $aicup22 $ai_cup_22
