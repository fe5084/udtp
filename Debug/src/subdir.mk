################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/file.cpp \
../src/main.cpp \
../src/packet.cpp \
../src/thread.cpp \
../src/udtp.cpp 

OBJS += \
./src/file.o \
./src/main.o \
./src/packet.o \
./src/thread.o \
./src/udtp.o 

CPP_DEPS += \
./src/file.d \
./src/main.d \
./src/packet.d \
./src/thread.d \
./src/udtp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


