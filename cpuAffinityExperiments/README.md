# Android CPU Affinity Experiments

Showing how to do cpu pinning on android.  
Tested on Huawei P30 (Kirin 980).

## Build && Run
0. Install `adb` tool with following command

```
sudo apt update
sudo apt install android-tools-adb android-tools-fastboot
```

1. Download [android ndk 15c](https://developer.android.com/ndk/downloads/older_releases.html)

2. Install cmake 3.11

3. Build the project by running the `build_android.sh` script and remember to modify the following parameter.

```
BUILD_ANDROID_NDK_HOME=/home/ldpe2g/Android/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE=/home/ldpe2g/Android/cmake-3.11.0-Linux-x86_64/bin/cmake
```

4. After successfully building the project, the executable `getCPUid` will be pushed to the folder `${DEPLOY_DIR}` on your android mobile and run. Then you should see the similar outputs, which depends on the device you are running, on your terminal.


```
#########################
iteration 1
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -360738256, running on cpu: 3
thread -358632912, running on cpu: 7
thread -357580240, running on cpu: 5
thread -359685584, running on cpu: 7
bind all core time: 76.996875

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -357580240, running on cpu: 2
thread -358632912, running on cpu: 3
thread -360738256, running on cpu: 1
thread -359685584, running on cpu: 0
bind little core time: 368.479166

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -359685584, running on cpu: 4
thread -358632912, running on cpu: 5
thread -360738256, running on cpu: 5
thread -357580240, running on cpu: 4
bind big core time: 99.261979

#########################
#########################
iteration 2
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -220220880, running on cpu: 1
thread -222326224, running on cpu: 0
thread -221273552, running on cpu: 4
thread -219168208, running on cpu: 5
bind all core time: 257.230729

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -219168208, running on cpu: 1
thread -220220880, running on cpu: 0
thread -221273552, running on cpu: 2
thread -222326224, running on cpu: 3
bind little core time: 269.061458

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -222326224, running on cpu: 4
thread -220220880, running on cpu: 5
thread -219168208, running on cpu: 4
thread -221273552, running on cpu: 5
bind big core time: 100.983854

#########################
#########################
iteration 3
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -386416080, running on cpu: 5
thread -387468752, running on cpu: 3
thread -389574096, running on cpu: 5
thread -388521424, running on cpu: 4
bind all core time: 281.139584

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -386416080, running on cpu: 3
thread -387468752, running on cpu: 0
thread -388521424, running on cpu: 1
thread -389574096, running on cpu: 2
bind little core time: 288.593229

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -388521424, running on cpu: 4
thread -389574096, running on cpu: 5
thread -387468752, running on cpu: 5
thread -386416080, running on cpu: 4
bind big core time: 99.023958

#########################
#########################
iteration 4
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -314076624, running on cpu: 2
thread -310918608, running on cpu: 5
thread -311971280, running on cpu: 7
thread -313023952, running on cpu: 7
bind all core time: 69.152604

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -310918608, running on cpu: 2
thread -311971280, running on cpu: 3
thread -314076624, running on cpu: 1
thread -313023952, running on cpu: 0
bind little core time: 356.026042

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -313023952, running on cpu: 4
thread -314076624, running on cpu: 5
thread -311971280, running on cpu: 5
thread -310918608, running on cpu: 4
bind big core time: 107.335416

#########################
#########################
iteration 5
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -177237456, running on cpu: 4
thread -174079440, running on cpu: 5
thread -175132112, running on cpu: 3
thread -176184784, running on cpu: 0
bind all core time: 275.303645

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -174079440, running on cpu: 2
thread -176184784, running on cpu: 0
thread -177237456, running on cpu: 3
thread -175132112, running on cpu: 1
bind little core time: 289.668229

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -176184784, running on cpu: 4
thread -177237456, running on cpu: 5
thread -174079440, running on cpu: 4
thread -175132112, running on cpu: 5
bind big core time: 97.390104

#########################
#########################
iteration 6
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -374889936, running on cpu: 4
thread -375942608, running on cpu: 5
thread -373837264, running on cpu: 3
thread -372784592, running on cpu: 4
bind all core time: 282.131250

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -372784592, running on cpu: 1
thread -373837264, running on cpu: 2
thread -374889936, running on cpu: 3
thread -375942608, running on cpu: 0
bind little core time: 287.757292

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -374889936, running on cpu: 4
thread -375942608, running on cpu: 5
thread -373837264, running on cpu: 5
thread -372784592, running on cpu: 4
bind big core time: 99.703125

#########################
#########################
iteration 7
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -285752784, running on cpu: 5
thread -288910800, running on cpu: 4
thread -286805456, running on cpu: 7
thread -287858128, running on cpu: 7
bind all core time: 69.031771

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -285752784, running on cpu: 0
thread -286805456, running on cpu: 2
thread -287858128, running on cpu: 3
thread -288910800, running on cpu: 2
bind little core time: 378.783333

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -287858128, running on cpu: 4
thread -288910800, running on cpu: 5
thread -286805456, running on cpu: 5
thread -285752784, running on cpu: 4
bind big core time: 98.843229

#########################
#########################
iteration 8
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -378027472, running on cpu: 5
thread -379080144, running on cpu: 5
thread -380132816, running on cpu: 4
thread -381185488, running on cpu: 5
bind all core time: 68.356771

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -378027472, running on cpu: 0
thread -379080144, running on cpu: 1
thread -380132816, running on cpu: 2
thread -381185488, running on cpu: 3
bind little core time: 292.199479

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -380132816, running on cpu: 4
thread -381185488, running on cpu: 5
thread -379080144, running on cpu: 5
thread -378027472, running on cpu: 4
bind big core time: 101.273958

#########################
#########################
iteration 9
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -311979472, running on cpu: 5
thread -310926800, running on cpu: 2
thread -308821456, running on cpu: 4
thread -309874128, running on cpu: 4
bind all core time: 258.637500

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -308821456, running on cpu: 3
thread -311979472, running on cpu: 2
thread -309874128, running on cpu: 0
thread -310926800, running on cpu: 1
bind little core time: 268.354688

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -310926800, running on cpu: 4
thread -311979472, running on cpu: 5
thread -309874128, running on cpu: 5
thread -308821456, running on cpu: 4
bind big core time: 108.800000

#########################
#########################
iteration 10
cpu numbers 8
before sort
cpu_0:1805000, cpu_1:1805000, cpu_2:1805000, cpu_3:1805000, cpu_4:1920000, cpu_5:1920000, cpu_6:2600000, cpu_7:2600000, 
after sort
cpu_6:2600000, cpu_7:2600000, cpu_4:1920000, cpu_5:1920000, cpu_2:1805000, cpu_3:1805000, cpu_0:1805000, cpu_1:1805000, 

bind all cores ex:
thread -316698064, running on cpu: 4
thread -314592720, running on cpu: 3
thread -315645392, running on cpu: 5
thread -313540048, running on cpu: 5
bind all core time: 285.434896

bind little cores ex:
bind cpu: 2, bind cpu: 3, bind cpu: 0, bind cpu: 1, 
thread -314592720, running on cpu: 1
thread -313540048, running on cpu: 3
thread -315645392, running on cpu: 0
thread -316698064, running on cpu: 3
bind little core time: 280.651042

bind big cores ex:
bind cpu: 6, bind cpu: 7, 
thread -315645392, running on cpu: 4
thread -316698064, running on cpu: 5
thread -314592720, running on cpu: 5
thread -313540048, running on cpu: 4
bind big core time: 97.480730

#########################
```
