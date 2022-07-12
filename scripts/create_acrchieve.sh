rm ../temp/aicup22.zip
find ../aicup22/aicup22 -type f -name "*.*pp" \
    ! -path "../aicup22/aicup22/codegame/*" \
    ! -path "../aicup22/aicup22/debugging/*" \
    ! -path "../aicup22/aicup22/model/*" \
    ! -name "CMakeLists.*" \
    ! -name "DebugInterface.*" \
    ! -name "Stream.*" \
    ! -name "TcpStream.*" \
    ! -name "main.*" \
    -print | zip -r ../temp/aicup22.zip -@
