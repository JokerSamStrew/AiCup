cmake --build ../aicup22/out
mkdir -p ../temp/local
../app/aicup22 --antialias false --config ../aicup22/tests/app_configs/auto_run_config.json \
    --batch-mode \
    --save-replay ../temp/local/replay.rep \
    --save-results ../temp/local/resulsts.txt &
aicup22=$!
sleep 2
../aicup22/out/ai_cup_22 &
ai_cup_22=$!
wait $aicup22 $ai_cup_22

../app/aicup22 --antialias false --replay ../temp/local/replay.rep
