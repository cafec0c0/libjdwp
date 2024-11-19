#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

// Version
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
  uint64_t thread;
} JdwpVirtualMachineAllThreadsThread;

typedef struct {
  uint32_t threads;
  JdwpVirtualMachineAllThreadsThread *threads_data;
} JdwpVirtualMachineAllThreadsData;

// Top level thread groups
typedef struct {
  uint64_t group;
} JdwpVirtualMachineTopLevelThreadGroupsGroup;

typedef struct {
  uint32_t groups;
  JdwpVirtualMachineTopLevelThreadGroupsGroup *groups_data;
} JdwpVirtualMachineTopLevelThreadGroupsData;

// Id sizes
typedef struct {
  uint32_t field_id_size;
  uint32_t method_id_size;
  uint32_t object_id_size;
  uint32_t reference_type_id_size;
  uint32_t frame_id_size;
} JdwpVirtualMachineIdSizesData;

// Exit
typedef struct {
  uint32_t exit_code;
} JdwpVirtualMachineExitCommand;

// Create String
typedef struct {
  char *utf;
} JdwpVirtualMachineCreateStringCommand;

typedef struct {
  uint64_t string_object;
} JdwpVirtualMachineCreateStringData;

// Capabilities
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
typedef void JdwpVirtualMachineClassPathsCommand;

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

// Capabilities new
typedef struct {
  uint8_t can_watch_field_modification;
  uint8_t can_watch_field_access;
  uint8_t can_get_bytecodes;
  uint8_t can_get_synthetic_attribute;
  uint8_t can_get_owned_monitor_info;
  uint8_t can_get_current_contended_monitor;
  uint8_t can_get_monitor_info;
  uint8_t can_redefine_classes;
  uint8_t can_add_method;
  uint8_t can_unrestrictedly_redefine_classes;
  uint8_t can_pop_frames;
  uint8_t can_use_instance_filters;
  uint8_t can_get_source_debug_extensions;
  uint8_t can_request_vm_death_event;
  uint8_t can_set_default_stratum;
  uint8_t can_get_instance_info;
  uint8_t can_request_monitor_events;
  uint8_t can_get_monitor_frame_info;
  uint8_t can_use_source_name_filters;
  uint8_t can_get_constant_pool;
  uint8_t can_force_early_return;
  uint8_t reserved_22;
  uint8_t reserved_23;
  uint8_t reserved_24;
  uint8_t reserved_25;
  uint8_t reserved_26;
  uint8_t reserved_27;
  uint8_t reserved_28;
  uint8_t reserved_29;
  uint8_t reserved_30;
  uint8_t reserved_31;
  uint8_t reserved_32;
} JdwpVirtualMachineCapabilitiesNewData;

typedef struct {
  uint64_t ref_type;
  uint32_t classfile;
  uint8_t *classfile_data;
} JdwpVirtualMachineRedefineClassesClassData;

// Redefine classes
typedef struct {
  uint32_t classes;
  JdwpVirtualMachineRedefineClassesClassData *classes_data;
} JdwpVirtualMachineRedefineClassesCommand;

// Set default stratum
typedef struct {
  char *stratum_id;
} JdwpVirtualMachineSetDefaultStratumCommand;

// All Classes with generic
typedef struct {
  JdwpTypeTag ref_type_tag;
  uint64_t type_id;
  char *signature;
  char *generic_signature;
  uint32_t status;
} JdwpVirtualMachineAllClassesWithGenericClassData;

typedef struct {
  uint32_t classes;
  JdwpVirtualMachineAllClassesWithGenericClassData *classes_data;
} JdwpVirtualMachineAllClassesWithGenericData;

// Instance counts
typedef struct {
  uint32_t ref_types_count;
  uint64_t *ref_types_data;
} JdwpVirtualMachineInstanceCountsCommand;

typedef struct {
  uint32_t counts;
  uint64_t *instance_count_data;
} JdwpVirtualMachineInstanceCountsData;

// All modules
typedef struct {
  uint32_t modules;
  uint64_t *modules_data;
} JdwpVirtualMachineAllModulesData;
#endif // VIRTUAL_MACHINE_H
