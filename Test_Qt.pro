TEMPLATE = subdirs

#SUBDIRS += \
#   PainterRotate \
#	FileTransfer

SUBDIRS += 	$$PWD/PainterRotate/PainterRotate.pro \
			$$PWD/FileTransfer/FileTransfer.pro \
			$$PWD/testDock/testDock.pro \
			$$PWD/DockExample/DockExample.pro \
			$$PWD/legendmarkers/legendmarkers.pro \
			$$PWD/zhexiantu/zhexiantu.pro \
			$$PWD/CusButtonGroup/CusButtonGroup.pro

CONFIG += ordered