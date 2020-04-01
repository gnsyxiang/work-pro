alsa-lib
========


### 配置系统

http://blog.csdn.net/yrj/article/details/4074184

ALSA的配置主文件默认是：/usr/share/alsa/alsa.conf

ALSA lib源代码中的conf.c负责load，解析这个配置文件。

配置以层次结构组织的，由一个snd_config_t 数据结构对象snd_config保存着总配置根，用list_head的方法将各个层次的配置组织起来。

```
struct _snd_config {  
	char *id;  
	snd_config_type_t type;  
	union {  
		long integer;  
		long long integer64;  
		char *string;  
		double real;  
		const void *ptr;  
		struct {  
			struct list_head fields;  
			int join;  
		} compound;  
	} u;  
	struct list_head list;  
	snd_config_t *father;  
	int hop;  
}; 
```

这个数据结构中的type的成员指定，此配置的属性，可以是一个值，也可以是个拥有下层配置的分支。类型如下，除了SND_CONFIG_TYPE_COMPOUND类型，其他类型不能有下层配置而拥有一个值。

```
/** Configuration node type. */  
typedef enum _snd_config_type {  
	/** Integer number. */  
	SND_CONFIG_TYPE_INTEGER,  
	/** 64 bit Integer number. */  
	SND_CONFIG_TYPE_INTEGER64,  
	/** Real number. */  
	SND_CONFIG_TYPE_REAL,  
	/** Character string. */  
	SND_CONFIG_TYPE_STRING,  
	/** Pointer (runtime only, cannot be saved). */  
	SND_CONFIG_TYPE_POINTER,  
	/** Compound node. */  
	SND_CONFIG_TYPE_COMPOUND = 1024  
} snd_config_type_t;
```









