#######################################################################
# Include the common makefiles:
#   - Variables:     Sets up the variables with some default values
include make_utils/common_variables.mk
#######################################################################

# Project Name
PROJECT_NAME = threading

# Add your souce directories here
SOURCE_DIRS = .

# Include paths
INC_DIRS = .
INC_DIRS += ..

#######################################################################
# Include the common makefiles:
include make_utils/common_shared_lib.mk
include make_utils/common_warnings.mk
include make_utils/common_var_autofill.mk
include make_utils/common_rules.mk
#######################################################################
