/*
 * Copyright (C) 2007-2008 Advanced Micro Devices, Inc.
 * Author: Joerg Roedel <joerg.roedel@amd.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef __LINUX_IOMMU_H
#define __LINUX_IOMMU_H

#include <linux/scatterlist.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/of.h>

#define IOMMU_READ	(1 << 0)
#define IOMMU_WRITE	(1 << 1)
#define IOMMU_CACHE	(1 << 2) /* DMA cache coherency */
#define IOMMU_NOEXEC	(1 << 3)
#define IOMMU_MMIO	(1 << 4) /* e.g. things like MSI doorbells */
/*
 * Where the bus hardware includes a privilege level as part of its access type
 * markings, and certain devices are capable of issuing transactions marked as
 * either 'supervisor' or 'user', the IOMMU_PRIV flag requests that the other
 * given permission flags only apply to accesses at the higher privilege level,
 * and that unprivileged transactions should have as little access as possible.
 * This would usually imply the same permissions as kernel mappings on the CPU,
 * if the IOMMU page table format is equivalent.
 */
#define IOMMU_PRIV	(1 << 5)
/* Use upstream device's bus attribute */
#define IOMMU_USE_UPSTREAM_HINT	(1 << 6)

/* Use upstream device's bus attribute with no write-allocate cache policy */
#define IOMMU_USE_LLC_NWA	(1 << 7)

/* For shoting entire IOMMU tlb once */
#define IOMMU_TLB_SHOT_ENTIRE	(1 << 6)

struct iommu_ops;
struct iommu_group;
struct bus_type;
struct device;
struct iommu_domain;
struct notifier_block;

/* iommu fault flags */
#define IOMMU_FAULT_READ                (1 << 0)
#define IOMMU_FAULT_WRITE               (1 << 1)
#define IOMMU_FAULT_TRANSLATION         (1 << 2)
#define IOMMU_FAULT_PERMISSION          (1 << 3)
#define IOMMU_FAULT_EXTERNAL            (1 << 4)
#define IOMMU_FAULT_TRANSACTION_STALLED (1 << 5)

typedef int (*iommu_fault_handler_t)(struct iommu_domain *,
			struct device *, unsigned long, int, void *);

struct iommu_domain_geometry {
	dma_addr_t aperture_start; /* First address that can be mapped    */
	dma_addr_t aperture_end;   /* Last address that can be mapped     */
	bool force_aperture;       /* DMA only allowed in mappable range? */
};

struct iommu_pgtbl_info {
	void *ops;
};

/* Domain feature flags */
#define __IOMMU_DOMAIN_PAGING	(1U << 0)  /* Support for iommu_map/unmap */
#define __IOMMU_DOMAIN_DMA_API	(1U << 1)  /* Domain for use in DMA-API
					      implementation              */
#define __IOMMU_DOMAIN_PT	(1U << 2)  /* Domain is identity mapped   */

/*
 * This are the possible domain-types
 *
 *	IOMMU_DOMAIN_BLOCKED	- All DMA is blocked, can be used to isolate
 *				  devices
 *	IOMMU_DOMAIN_IDENTITY	- DMA addresses are system physical addresses
 *	IOMMU_DOMAIN_UNMANAGED	- DMA mappings managed by IOMMU-API user, used
 *				  for VMs
 *	IOMMU_DOMAIN_DMA	- Internally used for DMA-API implementations.
 *				  This flag allows IOMMU drivers to implement
 *				  certain optimizations for these domains
 */
#define IOMMU_DOMAIN_BLOCKED	(0U)
#define IOMMU_DOMAIN_IDENTITY	(__IOMMU_DOMAIN_PT)
#define IOMMU_DOMAIN_UNMANAGED	(__IOMMU_DOMAIN_PAGING)
#define IOMMU_DOMAIN_DMA	(__IOMMU_DOMAIN_PAGING |	\
				 __IOMMU_DOMAIN_DMA_API)


#define IOMMU_DOMAIN_NAME_LEN 32
struct iommu_domain {
	unsigned type;
	const struct iommu_ops *ops;
	unsigned long pgsize_bitmap;	/* Bitmap of page sizes in use */
	iommu_fault_handler_t handler;
	void *handler_token;
	struct iommu_domain_geometry geometry;
	void *iova_cookie;
	bool is_debug_domain;
	char name[IOMMU_DOMAIN_NAME_LEN];
};

enum iommu_cap {
	IOMMU_CAP_CACHE_COHERENCY,	/* IOMMU can enforce cache coherent DMA
					   transactions */
	IOMMU_CAP_INTR_REMAP,		/* IOMMU supports interrupt isolation */
	IOMMU_CAP_NOEXEC,		/* IOMMU_NOEXEC flag */
};

/*
 * Following constraints are specifc to FSL_PAMUV1:
 *  -aperture must be power of 2, and naturally aligned
 *  -number of windows must be power of 2, and address space size
 *   of each window is determined by aperture size / # of windows
 *  -the actual size of the mapped region of a window must be power
 *   of 2 starting with 4KB and physical address must be naturally
 *   aligned.
 * DOMAIN_ATTR_FSL_PAMUV1 corresponds to the above mentioned contraints.
 * The caller can invoke iommu_domain_get_attr to check if the underlying
 * iommu implementation supports these constraints.
 *
 * DOMAIN_ATTR_NO_CFRE
 * Some bus implementations may enter a bad state if iommu reports an error
 * on context fault. As context faults are not always fatal, this must be
 * avoided.
 */

enum iommu_attr {
	DOMAIN_ATTR_GEOMETRY,
	DOMAIN_ATTR_PAGING,
	DOMAIN_ATTR_WINDOWS,
	DOMAIN_ATTR_FSL_PAMU_STASH,
	DOMAIN_ATTR_FSL_PAMU_ENABLE,
	DOMAIN_ATTR_FSL_PAMUV1,
	DOMAIN_ATTR_NESTING,	/* two stages of translation */
	DOMAIN_ATTR_PT_BASE_ADDR,
	DOMAIN_ATTR_CONTEXT_BANK,
	DOMAIN_ATTR_DYNAMIC,
	DOMAIN_ATTR_TTBR0,
	DOMAIN_ATTR_CONTEXTIDR,
	DOMAIN_ATTR_PROCID,
	DOMAIN_ATTR_NON_FATAL_FAULTS,
	DOMAIN_ATTR_S1_BYPASS,
	DOMAIN_ATTR_ATOMIC,
	DOMAIN_ATTR_SECURE_VMID,
	DOMAIN_ATTR_FAST,
	DOMAIN_ATTR_PGTBL_INFO,
	DOMAIN_ATTR_USE_UPSTREAM_HINT,
	DOMAIN_ATTR_EARLY_MAP,
	DOMAIN_ATTR_PAGE_TABLE_IS_COHERENT,
	DOMAIN_ATTR_PAGE_TABLE_FORCE_COHERENT,
	DOMAIN_ATTR_BITMAP_IOVA_ALLOCATOR,
	DOMAIN_ATTR_USE_LLC_NWA,
	DOMAIN_ATTR_FAULT_MODEL_NO_CFRE,
	DOMAIN_ATTR_FAULT_MODEL_NO_STALL,
	DOMAIN_ATTR_FAULT_MODEL_HUPCF,
	DOMAIN_ATTR_MAX,
};

/* These are the possible reserved region types */
enum iommu_resv_type {
	/* Memory regions which must be mapped 1:1 at all times */
	IOMMU_RESV_DIRECT,
	/* Arbitrary "never map this or give it to a device" address ranges */
	IOMMU_RESV_RESERVED,
	/* Hardware MSI region (untranslated) */
	IOMMU_RESV_MSI,
	/* Software-managed MSI translation window */
	IOMMU_RESV_SW_MSI,
};

/**
 * struct iommu_resv_region - descriptor for a reserved memory region
 * @list: Linked list pointers
 * @start: System physical start address of the region
 * @length: Length of the region in bytes
 * @prot: IOMMU Protection flags (READ/WRITE/...)
 * @type: Type of the reserved region
 */
struct iommu_resv_region {
	struct list_head	list;
	phys_addr_t		start;
	size_t			length;
	int			prot;
	enum iommu_resv_type	type;
};

extern struct dentry *iommu_debugfs_top;

#ifdef CONFIG_IOMMU_API

/**
 * struct iommu_ops - iommu ops and capabilities
 * @capable: check capability
 * @domain_alloc: allocate iommu domain
 * @domain_free: free iommu domain
 * @attach_dev: attach device to an iommu domain
 * @detach_dev: detach device from an iommu domain
 * @map: map a physically contiguous memory region to an iommu domain
 * @unmap: unmap a physically contiguous memory region from an iommu domain
 * @map_sg: map a scatter-gather list of physically contiguous memory chunks
 *          to an iommu domain
 * @flush_tlb_all: Synchronously flush all hardware TLBs for this domain
 * @tlb_range_add: Add a given iova range to the flush queue for this domain
 * @tlb_sync: Flush all queued ranges from the hardware TLBs and empty flush
 *            queue
 * @iova_to_phys: translate iova to physical address
 * @iova_to_phys_hard: translate iova to physical address using IOMMU hardware
 * @add_device: add device to iommu grouping
 * @remove_device: remove device from iommu grouping
 * @device_group: find iommu group for a particular device
 * @domain_get_attr: Query domain attributes
 * @domain_set_attr: Change domain attributes
 * @get_resv_regions: Request list of reserved regions for a device
 * @put_resv_regions: Free list of reserved regions for a device
 * @apply_resv_region: Temporary helper call-back for iova reserved ranges
 * @domain_window_enable: Configure and enable a particular window for a domain
 * @domain_window_disable: Disable a particular window for a domain
 * @domain_set_windows: Set the number of windows for a domain
 * @domain_get_windows: Return the number of windows for a domain
 * @of_xlate: add OF master IDs to iommu grouping
 * @pgsize_bitmap: bitmap of all possible supported page sizes
 * @trigger_fault: trigger a fault on the device attached to an iommu domain
 * @tlbi_domain: Invalidate all TLBs covering an iommu domain
 * @enable_config_clocks: Enable all config clocks for this domain's IOMMU
 * @disable_config_clocks: Disable all config clocks for this domain's IOMMU
 */
struct iommu_ops {
	bool (*capable)(enum iommu_cap);

	/* Domain allocation and freeing by the iommu driver */
	struct iommu_domain *(*domain_alloc)(unsigned iommu_domain_type);
	void (*domain_free)(struct iommu_domain *);

	int (*attach_dev)(struct iommu_domain *domain, struct device *dev);
	void (*detach_dev)(struct iommu_domain *domain, struct device *dev);
	int (*map)(struct iommu_domain *domain, unsigned long iova,
		   phys_addr_t paddr, size_t size, int prot);
	size_t (*unmap)(struct iommu_domain *domain, unsigned long iova,
		     size_t size);
	size_t (*map_sg)(struct iommu_domain *domain, unsigned long iova,
			 struct scatterlist *sg, unsigned int nents, int prot);
	void (*flush_iotlb_all)(struct iommu_domain *domain);
	void (*iotlb_range_add)(struct iommu_domain *domain,
				unsigned long iova, size_t size);
	void (*iotlb_sync)(struct iommu_domain *domain);
	phys_addr_t (*iova_to_phys)(struct iommu_domain *domain, dma_addr_t iova);
	phys_addr_t (*iova_to_phys_hard)(struct iommu_domain *domain,
					 dma_addr_t iova);
	int (*add_device)(struct device *dev);
	void (*remove_device)(struct device *dev);
	struct iommu_group *(*device_group)(struct device *dev);
	int (*domain_get_attr)(struct iommu_domain *domain,
			       enum iommu_attr attr, void *data);
	int (*domain_set_attr)(struct iommu_domain *domain,
			       enum iommu_attr attr, void *data);

	/* Request/Free a list of reserved regions for a device */
	void (*get_resv_regions)(struct device *dev, struct list_head *list);
	void (*put_resv_regions)(struct device *dev, struct list_head *list);
	void (*apply_resv_region)(struct device *dev,
				  struct iommu_domain *domain,
				  struct iommu_resv_region *region);

	/* Window handling functions */
	int (*domain_window_enable)(struct iommu_domain *domain, u32 wnd_nr,
				    phys_addr_t paddr, u64 size, int prot);
	void (*domain_window_disable)(struct iommu_domain *domain, u32 wnd_nr);
	/* Set the number of windows per domain */
	int (*domain_set_windows)(struct iommu_domain *domain, u32 w_count);
	/* Get the number of windows per domain */
	u32 (*domain_get_windows)(struct iommu_domain *domain);
	void (*trigger_fault)(struct iommu_domain *domain, unsigned long flags);
	void (*tlbi_domain)(struct iommu_domain *domain);
	int (*enable_config_clocks)(struct iommu_domain *domain);
	void (*disable_config_clocks)(struct iommu_domain *domain);
	uint64_t (*iova_to_pte)(struct iommu_domain *domain,
			 dma_addr_t iova);

	int (*of_xlate)(struct device *dev, struct of_phandle_args *args);
	bool (*is_attach_deferred)(struct iommu_domain *domain, struct device *dev);

	bool (*is_iova_coherent)(struct iommu_domain *domain, dma_addr_t iova);
	unsigned long pgsize_bitmap;
};

/**
 * struct iommu_device - IOMMU core representation of one IOMMU hardware
 *			 instance
 * @list: Used by the iommu-core to keep a list of registered iommus
 * @ops: iommu-ops for talking to this iommu
 * @dev: struct device for sysfs handling
 */
struct iommu_device {
	struct list_head list;
	const struct iommu_ops *ops;
	struct fwnode_handle *fwnode;
	struct device *dev;
};

int  iommu_device_register(struct iommu_device *iommu);
void iommu_device_unregister(struct iommu_device *iommu);
int  iommu_device_sysfs_add(struct iommu_device *iommu,
			    struct device *parent,
			    const struct attribute_group **groups,
			    const char *fmt, ...) __printf(4, 5);
void iommu_device_sysfs_remove(struct iommu_device *iommu);
int  iommu_device_link(struct iommu_device   *iommu, struct device *link);
void iommu_device_unlink(struct iommu_device *iommu, struct device *link);

static inline void iommu_device_set_ops(struct iommu_device *iommu,
					const struct iommu_ops *ops)
{
	iommu->ops = ops;
}

static inline void iommu_device_set_fwnode(struct iommu_device *iommu,
					   struct fwnode_handle *fwnode)
{
	iommu->fwnode = fwnode;
}

static inline struct iommu_device *dev_to_iommu_device(struct device *dev)
{
	return (struct iommu_device *)dev_get_drvdata(dev);
}

#define IOMMU_GROUP_NOTIFY_ADD_DEVICE		1 /* Device added */
#define IOMMU_GROUP_NOTIFY_DEL_DEVICE		2 /* Pre Device removed */
#define IOMMU_GROUP_NOTIFY_BIND_DRIVER		3 /* Pre Driver bind */
#define IOMMU_GROUP_NOTIFY_BOUND_DRIVER		4 /* Post Driver bind */
#define IOMMU_GROUP_NOTIFY_UNBIND_DRIVER	5 /* Pre Driver unbind */
#define IOMMU_GROUP_NOTIFY_UNBOUND_DRIVER	6 /* Post Driver unbind */

extern int bus_set_iommu(struct bus_type *bus, const struct iommu_ops *ops);
extern bool iommu_present(struct bus_type *bus);
extern bool iommu_capable(struct bus_type *bus, enum iommu_cap cap);
extern struct iommu_domain *iommu_domain_alloc(struct bus_type *bus);
extern struct iommu_group *iommu_group_get_by_id(int id);
extern void iommu_domain_free(struct iommu_domain *domain);
extern int iommu_attach_device(struct iommu_domain *domain,
			       struct device *dev);
extern void iommu_detach_device(struct iommu_domain *domain,
				struct device *dev);
extern struct iommu_domain *iommu_get_domain_for_dev(struct device *dev);
extern size_t iommu_pgsize(unsigned long pgsize_bitmap,
			   unsigned long addr_merge, size_t size);
extern int iommu_map(struct iommu_domain *domain, unsigned long iova,
		     phys_addr_t paddr, size_t size, int prot);
extern size_t iommu_unmap(struct iommu_domain *domain, unsigned long iova,
			  size_t size);
extern size_t iommu_unmap_fast(struct iommu_domain *domain,
			       unsigned long iova, size_t size);
extern size_t iommu_map_sg(struct iommu_domain *domain, unsigned long iova,
				struct scatterlist *sg, unsigned int nents,
				int prot);
extern size_t default_iommu_map_sg(struct iommu_domain *domain,
				   unsigned long iova,
				   struct scatterlist *sg, unsigned int nents,
				   int prot);
extern phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, dma_addr_t iova);
extern phys_addr_t iommu_iova_to_phys_hard(struct iommu_domain *domain,
					   dma_addr_t iova);
extern bool iommu_is_iova_coherent(struct iommu_domain *domain,
				dma_addr_t iova);
extern void iommu_set_fault_handler(struct iommu_domain *domain,
			iommu_fault_handler_t handler, void *token);

extern void iommu_get_resv_regions(struct device *dev, struct list_head *list);
extern void iommu_put_resv_regions(struct device *dev, struct list_head *list);
extern int iommu_request_dm_for_dev(struct device *dev);
extern struct iommu_resv_region *
iommu_alloc_resv_region(phys_addr_t start, size_t length, int prot,
			enum iommu_resv_type type);
extern int iommu_get_group_resv_regions(struct iommu_group *group,
					struct list_head *head);

extern int iommu_attach_group(struct iommu_domain *domain,
			      struct iommu_group *group);
extern void iommu_detach_group(struct iommu_domain *domain,
			       struct iommu_group *group);
extern struct iommu_group *iommu_group_alloc(void);
extern void *iommu_group_get_iommudata(struct iommu_group *group);
extern void iommu_group_set_iommudata(struct iommu_group *group,
				      void *iommu_data,
				      void (*release)(void *iommu_data));
extern int iommu_group_set_name(struct iommu_group *group, const char *name);
extern int iommu_group_add_device(struct iommu_group *group,
				  struct device *dev);
extern void iommu_group_remove_device(struct device *dev);
extern int iommu_group_for_each_dev(struct iommu_group *group, void *data,
				    int (*fn)(struct device *, void *));
extern struct iommu_group *iommu_group_get(struct device *dev);
extern struct iommu_group *iommu_group_ref_get(struct iommu_group *group);
extern void iommu_group_put(struct iommu_group *group);
extern int iommu_group_register_notifier(struct iommu_group *group,
					 struct notifier_block *nb);
extern int iommu_group_unregister_notifier(struct iommu_group *group,
					   struct notifier_block *nb);
extern int iommu_group_id(struct iommu_group *group);
extern struct iommu_group *iommu_group_get_for_dev(struct device *dev);
extern struct iommu_domain *iommu_group_default_domain(struct iommu_group *);

extern int iommu_domain_get_attr(struct iommu_domain *domain, enum iommu_attr,
				 void *data);
extern int iommu_domain_set_attr(struct iommu_domain *domain, enum iommu_attr,
				 void *data);

/* Window handling function prototypes */
extern int iommu_domain_window_enable(struct iommu_domain *domain, u32 wnd_nr,
				      phys_addr_t offset, u64 size,
				      int prot);
extern void iommu_domain_window_disable(struct iommu_domain *domain, u32 wnd_nr);

extern uint64_t iommu_iova_to_pte(struct iommu_domain *domain,
	    dma_addr_t iova);

extern int report_iommu_fault(struct iommu_domain *domain, struct device *dev,
			      unsigned long iova, int flags);

static inline void iommu_flush_tlb_all(struct iommu_domain *domain)
{
	if (domain->ops->flush_iotlb_all)
		domain->ops->flush_iotlb_all(domain);
}

static inline void iommu_tlb_range_add(struct iommu_domain *domain,
				       unsigned long iova, size_t size)
{
	if (domain->ops->iotlb_range_add)
		domain->ops->iotlb_range_add(domain, iova, size);
}

static inline void iommu_tlb_sync(struct iommu_domain *domain)
{
	if (domain->ops->iotlb_sync)
		domain->ops->iotlb_sync(domain);
}

extern void iommu_trigger_fault(struct iommu_domain *domain,
				unsigned long flags);

extern unsigned long iommu_reg_read(struct iommu_domain *domain,
				    unsigned long offset);
extern void iommu_reg_write(struct iommu_domain *domain, unsigned long offset,
			    unsigned long val);

/* PCI device grouping function */
extern struct iommu_group *pci_device_group(struct device *dev);
/* Generic device grouping function */
extern struct iommu_group *generic_device_group(struct device *dev);

static inline void iommu_tlbiall(struct iommu_domain *domain)
{
	if (domain->ops->tlbi_domain)
		domain->ops->tlbi_domain(domain);
}

static inline int iommu_enable_config_clocks(struct iommu_domain *domain)
{
	if (domain->ops->enable_config_clocks)
		return domain->ops->enable_config_clocks(domain);
	return 0;
}

static inline void iommu_disable_config_clocks(struct iommu_domain *domain)
{
	if (domain->ops->disable_config_clocks)
		domain->ops->disable_config_clocks(domain);
}

/**
 * struct iommu_fwspec - per-device IOMMU instance data
 * @ops: ops for this device's IOMMU
 * @iommu_fwnode: firmware handle for this device's IOMMU
 * @iommu_priv: IOMMU driver private data for this device
 * @num_ids: number of associated device IDs
 * @ids: IDs which this device may present to the IOMMU
 */
struct iommu_fwspec {
	const struct iommu_ops	*ops;
	struct fwnode_handle	*iommu_fwnode;
	void			*iommu_priv;
	unsigned int		num_ids;
	u32			ids[1];
};

int iommu_fwspec_init(struct device *dev, struct fwnode_handle *iommu_fwnode,
		      const struct iommu_ops *ops);
void iommu_fwspec_free(struct device *dev);
int iommu_fwspec_add_ids(struct device *dev, u32 *ids, int num_ids);
const struct iommu_ops *iommu_ops_from_fwnode(struct fwnode_handle *fwnode);
int iommu_is_available(struct device *dev);

#else /* CONFIG_IOMMU_API */

struct iommu_ops {};
struct iommu_group {};
struct iommu_fwspec {};
struct iommu_device {};

static inline bool iommu_present(struct bus_type *bus)
{
	return false;
}

static inline bool iommu_capable(struct bus_type *bus, enum iommu_cap cap)
{
	return false;
}

static inline struct iommu_domain *iommu_domain_alloc(struct bus_type *bus)
{
	return NULL;
}

static inline struct iommu_group *iommu_group_get_by_id(int id)
{
	return NULL;
}

static inline void iommu_domain_free(struct iommu_domain *domain)
{
}

static inline int iommu_attach_device(struct iommu_domain *domain,
				      struct device *dev)
{
	return -ENODEV;
}

static inline void iommu_detach_device(struct iommu_domain *domain,
				       struct device *dev)
{
}

static inline struct iommu_domain *iommu_get_domain_for_dev(struct device *dev)
{
	return NULL;
}

static inline int iommu_map(struct iommu_domain *domain, unsigned long iova,
			    phys_addr_t paddr, size_t size, int prot)
{
	return -ENODEV;
}

static inline size_t iommu_unmap(struct iommu_domain *domain,
				 unsigned long iova, size_t size)
{
	return 0;
}

static inline size_t iommu_unmap_fast(struct iommu_domain *domain,
				      unsigned long iova, int gfp_order)
{
	return 0;
}

static inline size_t iommu_map_sg(struct iommu_domain *domain,
				  unsigned long iova, struct scatterlist *sg,
				  unsigned int nents, int prot)
{
	return 0;
}

static inline void iommu_flush_tlb_all(struct iommu_domain *domain)
{
}

static inline void iommu_tlb_range_add(struct iommu_domain *domain,
				       unsigned long iova, size_t size)
{
}

static inline void iommu_tlb_sync(struct iommu_domain *domain)
{
}

static inline int iommu_domain_window_enable(struct iommu_domain *domain,
					     u32 wnd_nr, phys_addr_t paddr,
					     u64 size, int prot)
{
	return -ENODEV;
}

static inline void iommu_domain_window_disable(struct iommu_domain *domain,
					       u32 wnd_nr)
{
}

static inline phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, dma_addr_t iova)
{
	return 0;
}

static inline phys_addr_t iommu_iova_to_phys_hard(struct iommu_domain *domain,
						  dma_addr_t iova)
{
	return 0;
}

static inline bool iommu_is_iova_coherent(struct iommu_domain *domain,
					  dma_addr_t iova)
{
	return 0;
}

static inline void iommu_set_fault_handler(struct iommu_domain *domain,
				iommu_fault_handler_t handler, void *token)
{
}

static inline void iommu_get_resv_regions(struct device *dev,
					struct list_head *list)
{
}

static inline void iommu_put_resv_regions(struct device *dev,
					struct list_head *list)
{
}

static inline int iommu_get_group_resv_regions(struct iommu_group *group,
					       struct list_head *head)
{
	return -ENODEV;
}

static inline int iommu_request_dm_for_dev(struct device *dev)
{
	return -ENODEV;
}

static inline int iommu_attach_group(struct iommu_domain *domain,
				     struct iommu_group *group)
{
	return -ENODEV;
}

static inline void iommu_detach_group(struct iommu_domain *domain,
				      struct iommu_group *group)
{
}

static inline struct iommu_group *iommu_group_alloc(void)
{
	return ERR_PTR(-ENODEV);
}

static inline void *iommu_group_get_iommudata(struct iommu_group *group)
{
	return NULL;
}

static inline void iommu_group_set_iommudata(struct iommu_group *group,
					     void *iommu_data,
					     void (*release)(void *iommu_data))
{
}

static inline int iommu_group_set_name(struct iommu_group *group,
				       const char *name)
{
	return -ENODEV;
}

static inline int iommu_group_add_device(struct iommu_group *group,
					 struct device *dev)
{
	return -ENODEV;
}

static inline void iommu_group_remove_device(struct device *dev)
{
}

static inline int iommu_group_for_each_dev(struct iommu_group *group,
					   void *data,
					   int (*fn)(struct device *, void *))
{
	return -ENODEV;
}

static inline struct iommu_group *iommu_group_get(struct device *dev)
{
	return NULL;
}

static inline void iommu_group_put(struct iommu_group *group)
{
}

static inline int iommu_group_register_notifier(struct iommu_group *group,
						struct notifier_block *nb)
{
	return -ENODEV;
}

static inline int iommu_group_unregister_notifier(struct iommu_group *group,
						  struct notifier_block *nb)
{
	return 0;
}

static inline int iommu_group_id(struct iommu_group *group)
{
	return -ENODEV;
}

static inline int iommu_domain_get_attr(struct iommu_domain *domain,
					enum iommu_attr attr, void *data)
{
	return -EINVAL;
}

static inline int iommu_domain_set_attr(struct iommu_domain *domain,
					enum iommu_attr attr, void *data)
{
	return -EINVAL;
}

static inline int  iommu_device_register(struct iommu_device *iommu)
{
	return -ENODEV;
}

static inline void iommu_device_set_ops(struct iommu_device *iommu,
					const struct iommu_ops *ops)
{
}

static inline void iommu_device_set_fwnode(struct iommu_device *iommu,
					   struct fwnode_handle *fwnode)
{
}

static inline struct iommu_device *dev_to_iommu_device(struct device *dev)
{
	return NULL;
}

static inline void iommu_device_unregister(struct iommu_device *iommu)
{
}

static inline int  iommu_device_sysfs_add(struct iommu_device *iommu,
					  struct device *parent,
					  const struct attribute_group **groups,
					  const char *fmt, ...)
{
	return -ENODEV;
}

static inline void iommu_device_sysfs_remove(struct iommu_device *iommu)
{
}

static inline int iommu_device_link(struct device *dev, struct device *link)
{
	return -EINVAL;
}

static inline void iommu_device_unlink(struct device *dev, struct device *link)
{
}

static inline void iommu_trigger_fault(struct iommu_domain *domain,
				       unsigned long flags)
{
}

static inline unsigned long iommu_reg_read(struct iommu_domain *domain,
					   unsigned long offset)
{
	return 0;
}

static inline void iommu_reg_write(struct iommu_domain *domain,
				   unsigned long val, unsigned long offset)
{
}

static inline void iommu_tlbiall(struct iommu_domain *domain)
{
}

static inline int iommu_enable_config_clocks(struct iommu_domain *domain)
{
	return 0;
}

static inline void iommu_disable_config_clocks(struct iommu_domain *domain)
{
}

static inline int iommu_fwspec_init(struct device *dev,
				    struct fwnode_handle *iommu_fwnode,
				    const struct iommu_ops *ops)
{
	return -ENODEV;
}

static inline void iommu_fwspec_free(struct device *dev)
{
}

static inline int iommu_fwspec_add_ids(struct device *dev, u32 *ids,
				       int num_ids)
{
	return -ENODEV;
}

static inline
const struct iommu_ops *iommu_ops_from_fwnode(struct fwnode_handle *fwnode)
{
	return NULL;
}

static inline int iommu_is_available(struct device *dev)
{
	return -ENODEV;
}
#endif /* CONFIG_IOMMU_API */

#ifdef CONFIG_IOMMU_DEBUGFS
extern	struct dentry *iommu_debugfs_dir;
void iommu_debugfs_setup(void);
#else
static inline void iommu_debugfs_setup(void) {}
#endif

#endif /* __LINUX_IOMMU_H */
