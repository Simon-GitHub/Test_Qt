TEMPLATE = subdirs

#SUBDIRS += \
#   PainterRotate \
#	FileTransfer

SUBDIRS += 	$$PWD/PainterRotate/PainterRotate.pro \
			$$PWD/FileTransfer/FileTransfer.pro \
			$$PWD/testDock/testDock.pro \
			$$PWD/DockExample/DockExample.pro

CONFIG += ordered