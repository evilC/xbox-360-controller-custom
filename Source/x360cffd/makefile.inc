.SUFFIXES: .inx

STAMP=stampinf

$(OBJ_PATH)\$(O)\$(TARGETNAME).inf:
    copy $(@B).inx $@
    $(STAMP) -f $@ -a $(_BUILDARCH) -k $(KMDF_VERSION).$(KMDF_MINOR_VERSION) -u $(UMDF_VERSION).$(UMDF_MINOR_VERSION).0

