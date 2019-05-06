#include <linux/init.h>
#include <linux/frontswap.h>
#include <linux/mm_inline.h>
#include <linux/migrate.h>
#include <linux/pagemap.h>
#include <linux/swapops.h>

#include "internal.h"

static void demote_frontswap_invalidate_area(unsigned type)
{
}

static void demote_frontswap_invalidate_page(unsigned type, pgoff_t offset)
{
}

static int demote_frontswap_load(unsigned type, pgoff_t offset,
				struct page *page)
{
	return -1;
}

static int demote_frontswap_store(unsigned type, pgoff_t offset,
				  struct page *page)
{
	int ret;

	/* Dirty was cleared for IO. Temporarily set it for migration */
	SetPageDirty(page);

	ret = migrate_demote_mapping(page);
	if (ret != MIGRATEPAGE_SUCCESS)
		ClearPageDirty(page);

	return ret;
}

static void demote_frontswap_init(unsigned type)
{
}

static struct frontswap_ops demote_frontswap_ops = {
	.init			= demote_frontswap_init,
	.load			= demote_frontswap_load,
	.store			= demote_frontswap_store,
	.invalidate_page	= demote_frontswap_invalidate_page,
	.invalidate_area	= demote_frontswap_invalidate_area,
};

static int __init init_demote(void)
{
	frontswap_register_ops(&demote_frontswap_ops);

	return 0;
}

late_initcall(init_demote);
