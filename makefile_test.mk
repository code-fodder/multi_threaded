#######################################################################
# Include the common makefiles:
#   - Variables:     Sets up the variables with some default values
include make_utils/common_variables.mk
#######################################################################

# Project Name
PROJECT_NAME = multi_thread

LIB_DEPS += threading/lib/libthreading_$(TARGET)$(BUILD_SUFFIX).so

POST_BUILD_TASKS += export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH_VAL);
POST_BUILD_TASKS += cd $(OUTPUT_DIR) && ./$(OUTPUT_FILE);

#######################################################################
# Include the common makefiles:
include make_utils/common_executable.mk
include make_utils/common_warnings.mk
include make_utils/common_var_autofill.mk
include make_utils/common_rules.mk
#######################################################################
link_cmd =