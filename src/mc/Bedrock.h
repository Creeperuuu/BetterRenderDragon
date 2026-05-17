#pragma once

#include "mc/_HeaderOutputPredefine.h"

namespace Bedrock {
    enum class LogAreaID : int {
        LogAreaAll                 = 0,
        LogAreaPlatform            = 1,
        LogAreaEntity              = 2,
        LogAreaDatabase            = 3,
        LogAreaGui                 = 4,
        LogAreaSystem              = 5,
        LogAreaNetwork             = 6,
        LogAreaRender              = 7,
        LogAreaMemory              = 8,
        LogAreaAnimation           = 9,
        LogAreaInput               = 10,
        LogAreaLevel               = 11,
        LogAreaServer              = 12,
        LogAreaDlc                 = 13,
        LogAreaPhysics             = 14,
        LogAreaFile                = 15,
        LogAreaStorage             = 16,
        LogAreaRealms              = 17,
        LogAreaRealmsapi           = 18,
        LogAreaXboxlive            = 19,
        LogAreaUsermanager         = 20,
        LogAreaXsapi               = 21,
        LogAreaPerf                = 22,
        LogAreaTelemetry           = 23,
        LogAreaBlocks              = 24,
        LogAreaRaknet              = 25,
        LogAreaGameface            = 26,
        LogAreaSound               = 27,
        LogAreaInteractive         = 28,
        LogAreaScripting           = 29,
        LogAreaPlayfab             = 30,
        LogAreaAutomation          = 31,
        LogAreaPersona             = 32,
        LogAreaTexture             = 33,
        LogAreaAssetpackages       = 34,
        LogAreaItems               = 35,
        LogAreaServices            = 36,
        LogAreaVolumes             = 37,
        LogAreaLoottable           = 38,
        LogAreaSidebar             = 39,
        LogAreaLocalization        = 40,
        LogAreaMovement            = 41,
        LogAreaLiveevents          = 42,
        LogAreaEditor              = 43,
        LogAreaLeveltransition     = 44,
        LogAreaUnknown             = 45,
        LogAreaStore               = 46,
        LogAreaWorld               = 47,
        LogAreaMessaging           = 48,
        LogAreaNethernet           = 49,
        LogAreaSerialization       = 50,
        LogAreaDisconnectionscreen = 51,
        NumLogAreas                = 52,
    };
    enum class LogLevel : uchar {
        Info    = 0,
        Warning = 1,
        Error   = 2,
        Debug   = 3,
    };
struct CallStack {
public:
    // CallStack inner types declare
    // clang-format off
    struct Context;
    struct Frame;
    struct FrameWithContext;
    // clang-format on

    // CallStack inner types define
    struct Context {
    public:
        // member variables
        // NOLINTBEGIN
        ::std::string                        mValue;
        ::std::optional<::Bedrock::LogLevel> mLogLevel;
        ::std::optional<::Bedrock::LogAreaID>         mLogArea;
        // NOLINTEND
    };

    struct Frame {
    public:
        // member variables
        // NOLINTBEGIN
        uint64             mFilenameHash;
        ::std::string_view mFilename;
        uint               mLine;
        // NOLINTEND
    };

    struct FrameWithContext {
    public:
        // member variables
        // NOLINTBEGIN
        ::Bedrock::CallStack::Frame                    mFrame;
        ::std::optional<::Bedrock::CallStack::Context> mContext;
        // NOLINTEND

    public:
        FrameWithContext& operator=(FrameWithContext const&) = default;
        FrameWithContext(FrameWithContext const&)            = default;

        // prevent constructor by default
        FrameWithContext();
    };

public:
    // member variables
    // NOLINTBEGIN
    ::std::vector<::Bedrock::CallStack::FrameWithContext> mFrames;
    // NOLINTEND

};

    template <typename E = std::error_code>
    struct ErrorInfo {
    public:
        ErrorInfo() = default;

        ErrorInfo(ErrorInfo const&)                = default;
        ErrorInfo& operator=(ErrorInfo const&)     = default;
        ErrorInfo(ErrorInfo&&) noexcept            = default;
        ErrorInfo& operator=(ErrorInfo&&) noexcept = default;

        ErrorInfo(E const& error) : mError(error) {}

        [[nodiscard]] constexpr E&       code() { return mError; }
        [[nodiscard]] constexpr E const& code() const { return mError; }

        E                      mError;
        CallStack              mCallStack;
        std::vector<ErrorInfo> mStackErrors;
    };

    template <typename T, typename Err>
    class Result : public nonstd::expected<T, ErrorInfo<Err>> {
    public:
        using Base = nonstd::expected<T, ErrorInfo<Err>>;
        using Base::Base;
    };
}; // namespace Bedrock