TEMPLATE = subdirs

SUBDIRS += \
    3rdparty/qtcsv \
    integrity \
    pf_core \
    pf_performance

# Use .depends to specify that a project depends on another.
pf_performance.depends = qtcsv
pf_performance.depends = pf_core
