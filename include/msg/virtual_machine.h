#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

// Version
typedef struct {
} JdwpVirtualMachineVersionCommand;

typedef struct {
  char *description;
  uint32_t jdwp_major;
  uint32_t jdwp_minor;
  char *vm_version;
  char *vm_name;
} JdwpVirtualMachineVersionData;

// Classes by signature
typedef struct {
  char *signature;
} JdwpVirtualMachineClassesBySignatureCommand;

typedef struct {
  JdwpTypeTag ref_type_tag;
  uint64_t type_id;
  uint32_t status;
} JdwpVirtualMachineClassesBySignatureClassData;

typedef struct {
  size_t classes;
  JdwpVirtualMachineClassesBySignatureClassData *classes_data;
} JdwpVirtualMachineClassesBySignatureData;

// All classes
typedef struct {
} JdwpVirtualMachineAllClassesCommand;

typedef struct {
  JdwpTypeTag ref_type_tag;
  uint64_t type_id;
  char *signature;
  uint32_t status;
} JdwpVirtualMachineAllClassesClassData;

typedef struct {
  uint32_t classes;
  JdwpVirtualMachineAllClassesClassData *classes_data;
} JdwpVirtualMachineAllClassesData;

// All threads
typedef struct {
} JdwpVirtualMachineAllThreadsCommand;

typedef struct {
  uint64_t thread;
} JdwpVirtualMachineAllThreadsThread;

typedef struct {
  uint32_t threads;
  JdwpVirtualMachineAllThreadsThread *threads_data;
} JdwpVirtualMachineAllThreadsData;

// Top level thread groups
typedef struct {
} JdwpVirtualMachineTopLevelThreadGroupsCommand;

typedef struct {
  uint64_t group;
} JdwpVirtualMachineTopLevelThreadGroupsGroup;

typedef struct {
  uint32_t groups;
  JdwpVirtualMachineTopLevelThreadGroupsGroup *groups_data;
} JdwpVirtualMachineTopLevelThreadGroupsData;

// Dispose
typedef struct {
} JdwpVirtualMachineDisposeCommand;

typedef struct {
} JdwpVirtualMachineDisposeData;

// Id sizes
typedef struct {
} JdwpVirtualMachineIdSizesCommand;

typedef struct {
  uint32_t field_id_size;
  uint32_t method_id_size;
  uint32_t object_id_size;
  uint32_t reference_type_id_size;
  uint32_t frame_id_size;
} JdwpVirtualMachineIdSizesData;

// Suspend
typedef struct {
} JdwpVirtualMachineSuspendCommand;

typedef struct {
} JdwpVirtualMachineSuspendData;

// Resume
typedef struct {
} JdwpVirtualMachineResumeCommand;

typedef struct {
} JdwpVirtualMachineResumeData;

// Resume
typedef struct {
  uint32_t exit_code;
} JdwpVirtualMachineExitCommand;

typedef struct {
} JdwpVirtualMachineExitData;

// Create String
typedef struct {
  char *utf;
} JdwpVirtualMachineCreateStringCommand;

typedef struct {
  uint64_t string_object;
} JdwpVirtualMachineCreateStringData;

// Capabilities
typedef struct {
} JdwpVirtualMachineCapabilitiesCommand;

typedef struct {
  uint8_t can_watch_field_modification;
  uint8_t can_watch_field_access;
  uint8_t can_get_bytecodes;
  uint8_t can_get_synthetic_attribute;
  uint8_t can_get_owned_monitor_info;
  uint8_t can_get_current_contended_monitor;
  uint8_t can_get_monitor_info;
} JdwpVirtualMachineCapabilitiesData;

// Class Paths
typedef struct {
} JdwpVirtualMachineClassPathsCommand;

typedef struct {
  char *base_dir;
  uint32_t class_paths;
  char **class_paths_data;
  uint32_t boot_class_paths;
  char **boot_class_paths_data;
} JdwpVirtualMachineClassPathsData;

// Dispose Objects
typedef struct {
  uint64_t object;
  uint32_t ref_cnt;
} JdwpVirtualMachineDisposeObjectsObjectData;

typedef struct {
  uint32_t requests;
  JdwpVirtualMachineDisposeObjectsObjectData *requests_data;
} JdwpVirtualMachineDisposeObjectsCommand;

typedef struct {
} JdwpVirtualMachineDisposeObjectsData;

#endif // VIRTUAL_MACHINE_H
