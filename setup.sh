export PROJECT=`pwd`/../

if [ "$1" == "x86_64" ]; then
    echo "Assuming x86_64 architecture"
    export ARCH=x86_64
    export CROSS_PREFIX=""
else
    echo "Assuming ARM architecture"
    export VIVADO_HOME=/opt/Xilinx/Vivado/2015.3
    source $VIVADO_HOME/settings64.sh
    export ARCH=arm
    export TOOLCHAIN=/opt/Xilinx/SDK/2015.3
    export CROSS_PREFIX=arm-xilinx-linux-gnueabi-
    export PATH=$TOOLCHAIN/gnu/arm/lin/bin:$TOOLCHAIN/bin/:$PATH
fi

export JAVA_HOME=/usr/lib/jvm/java-8-oracle
export JDK_HOME=/usr/lib/jvm/java-8-oracle


