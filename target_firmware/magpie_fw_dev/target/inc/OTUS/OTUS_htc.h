#ifndef __AR6K_HTC_H__
#define __AR6K_HTC_H__

/* Host/Target Communications for an AR6K Target */

/* Number of mailboxes */
#define AR6000_MBOX_COUNT  4

/* ------ MBOX ID ------ */
typedef enum
{
    ENDPOINT_UNUSED = -1,
    ENDPOINT1 = 0,
    ENDPOINT2,
    ENDPOINT3,
    ENDPOINT4,
} HTC_ENDPOINT_ID;


/* An AR6000 DMA Descriptor. See HTC_descriptor(). */
struct AR6000_DMA_desc_s {
    A_UINT32                  dma_control;
    char                     *dma_data;
    struct AR6000_DMA_desc_s *dma_next;
};


/*
 * Make a Mailbox DMA descriptor available to HTC.
 *
 * Once made available, there is no way to reclaim this memory.
 * The caller must guarantee that a descriptor exists for each
 * buffer that is used to send or receive.  It is expected that
 * the caller will supply a bunch of descriptors once during
 * initialization, and then forget about them.  The number of
 * buffers given to HTC for send+recv must never exceed the
 * number of descriptors given to HTC.
 *
 * HTC accesses descriptors via uncached accesses.  The caller
 * must guarantee not to place any other data in the same cache
 * line as a DMA descriptor!  In practice, this means that the
 * caller should allocate a block of memory for descriptors,
 * and the block should include padding at the start and end
 * to guarantee there will be no other data in the same cache
 * line.
 *
 * It would be far preferable to bury descriptors in the bufinfo
 * structure; but there are practical issues that prevent this.
 * It turns out that the most efficient way to add descriptors
 * to an active DMA engine requires HTC to "own and actively
 * manage" the descriptors.  HTC needs to make the association
 * between descriptors and buffers at the last possible moment.
 *
 * extern void _HTC_descriptor(struct AR6000_DMA_desc_s *descriptor);
 */

/*
 * The following interfaces make it easy to allocate suitable
 * descriptors for HTC.  During initialization, simply use the
 * HTC_DESCRIPTORS_INIT macro and specify the number of descriptors
 * desired.  This number must be a constant, since it is used to
 * declare a static array!
 *
 * The descriptor array is padded with a cache line at the start
 * and another at the end.  This avoids false sharing between adjacent
 * cached data and uncached descriptors.
 */
#define HTC_DESCRIPTOR_SPACE_SIZE(ndescs)                                   \
    (((ndescs) * sizeof(struct AR6000_DMA_desc_s)) + 2*A_CACHE_LINE_SIZE)

#define HTC_DESCRIPTORS_INIT(ndescs)                                        \
{                                                                           \
    static A_UINT8 HTC_descriptor_space[HTC_DESCRIPTOR_SPACE_SIZE(ndescs)]; \
    struct AR6000_DMA_desc_s *desc;                                         \
    int i;                                                                  \
                                                                            \
    A_DATA_CACHE_FLUSH(HTC_descriptor_space, sizeof(HTC_descriptor_space)); \
                                                                            \
    desc = (struct AR6000_DMA_desc_s *)                                     \
        A_ROUND_UP((A_UINT32)HTC_descriptor_space, A_CACHE_LINE_SIZE);      \
                                                                            \
    for (i=0; i<(ndescs); i++) {                                            \
        HTC_descriptor(desc);                                               \
        desc++;                                                             \
    }                                                                       \
}

#endif /* __AR6K_HTC_H__ */
