#ifndef SLIVER_ART_METHOD
#define SLIVER_ART_METHOD

/**
 * 结构 可参考 :https://github.com/liaoguobao/javahook/blob/30b4c1eb134d864685583e5cf3b1667c30349c30/native/javahook/src/JavaHook/ArtMethod.h
 */
class ArtMethod final {
public:
    // Field order required by test "ValidateFieldOrderOfJavaCppUnionClasses".
    // The class we are a part of.
    //GcRoot<mirror::Class> declaring_class_;
    uint32_t declaring_class_;

    // Access flags; low 16 bits are defined by spec.
    // Getting and setting this flag needs to be atomic when concurrency is
    // possible, e.g. after this method's class is linked. Such as when setting
    // verifier flags and single-implementation flag.
    //std::atomic<std::uint32_t> access_flags_;
    uint32_t access_flags_;

    /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */

    // Index into method_ids of the dex file associated with this method.
    uint32_t dex_method_index_;

    /* End of dex file fields. */

    // Entry within a dispatch table for this method. For static/direct methods the index is into
    // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
    // ifTable.
    uint16_t method_index_;

    union {
        // Non-abstract methods: The hotness we measure for this method. Not atomic,
        // as we allow missing increments: if the method is hot, we will see it eventually.
        uint16_t hotness_count_;
        // Abstract methods: IMT index.
        uint16_t imt_index_;
    };


public:
    static constexpr uint32_t kRuntimeMethodDexMethodIndex = 0xFFFFFFFF;

     inline bool IsRuntimeMethod() const{
        return dex_method_index_ == kRuntimeMethodDexMethodIndex;
    }

};
#endif
