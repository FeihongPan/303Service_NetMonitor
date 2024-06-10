TEMPLATE = subdirs

SUBDIRS += \
    CommonDLL \
    MainProject

CommonDLL.subdir = $$_PRO_FILE_PWD_/CommonDLL
MainProject.subdir = $$_PRO_FILE_PWD_/MainProject

MainProject.depends = CommonDLL
