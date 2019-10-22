cd ./src
ln -s rpc /home/zouqing/work/carnet/linux/auto/vmp/vmp-rl/src/rpc


compile
open project onion:
  cd onion/src
  ln -s smp /home/zouqing/work/carnet/linux/auto/vmp/vmp-sm
  export DEPEND_PATH=/home/zouqing/work/carnet/linux/auto/vmp/premake/ubuntu/install
  export DEPEND_PROTOBUF_C_PATH=/home/zouqing/osource/network/pc/final_protobuf
  cmake -DCMAKE_BUILD_TYPE=Debug -DONION_POLLER=libevent ..

