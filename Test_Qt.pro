TEMPLATE = subdirs

#SUBDIRS += \
#   PainterRotate \
#	FileTransfer

SUBDIRS += 	$$PWD/PainterRotate/PainterRotate.pro \
			$$PWD/FileTransfer/FileTransfer.pro

CONFIG += ordered