make -C ../aicup22/out
mkdir -p ../temp/local
../app/aicup22 --config ../aicup22/tests/app_configs/auto_run_config.json \
    --batch-mode \
    --save-replay ../temp/local/replay.rep \
    --save-results ../temp/local/resulsts.txt
../app/aicup22 --replay ../temp/local/replay.rep
