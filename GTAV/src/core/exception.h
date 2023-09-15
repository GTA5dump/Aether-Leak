#pragma once
#include "pch.h"
#include "util/Log.h"

namespace base::core::exception::handler {

    enum class exception_access_vioaltion_types : ULONG_PTR {
        READ_INACCESSIBLE_DATA = 0,
        WRITE_INACCESSIBLE_DATA = 1,
        EXECUTE_NON_EXECUTABLE_DATA = 8
    };

    enum class exception_in_page_error_types : unsigned int {
        READ_ACCESS = 0,
        WRITE_ACCESS = 1,
        EXECUTE_ACCESS = 8,
        GUARD_PAGE = 0x80000001,
        DEP = 0x80000002
    };

    inline std::string get_stack_trace()
    {
        std::ostringstream stack_trace{};
        stack_trace << "[";
        constexpr int max_callers = 62;

        const auto capture_stack_back_trace = [](ULONG frames_to_skip, ULONG frames_to_capture, PVOID* call_stack, PULONG frames_captured) -> USHORT {
            typedef USHORT(WINAPI* capture_stack_back_trace_type)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
            static const auto capture_stack_back_trace_fn = reinterpret_cast<capture_stack_back_trace_type>(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));
            if (!capture_stack_back_trace_fn) { return 0; }
            return capture_stack_back_trace_fn(frames_to_skip, frames_to_capture, call_stack, frames_captured);
        };

        PVOID call_stack[max_callers];
        const int count = capture_stack_back_trace(1, max_callers, call_stack, nullptr);

        for (int i = 0; i < count; i++) {
            const auto caller_addr = reinterpret_cast<uintptr_t>(call_stack[i]);
            const auto flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
            HMODULE current_caller_module = nullptr;

            if (GetModuleHandleEx(flags, reinterpret_cast<LPCWSTR>(caller_addr), &current_caller_module)) {
                char module_name[MAX_PATH];
                if (GetModuleBaseNameA(GetCurrentProcess(), current_caller_module, module_name, MAX_PATH)) {
                    stack_trace << "0x" << std::hex << (caller_addr - reinterpret_cast<uintptr_t>(current_caller_module)) << " [" << module_name << "]" << (i != count - 1 ? ", " : "");
                }
                else {
                    stack_trace << "0x" << std::hex << caller_addr << (i != count - 1 ? ", " : "");
                }
            }
            else {
                stack_trace << "0x" << std::hex << caller_addr << (i != count - 1 ? ", " : "");
            }
        }
        stack_trace << "]";

        return stack_trace.str();
    }

    inline LONG WINAPI exception_handler(EXCEPTION_POINTERS* exc_info) {

        const auto exc_code = exc_info->ExceptionRecord->ExceptionCode;
        const auto exc_address = exc_info->ExceptionRecord->ExceptionAddress;
        const auto exc_access_violation = exc_info->ExceptionRecord->ExceptionInformation[0];
        const auto exc_access_address = exc_info->ExceptionRecord->ExceptionInformation[1];
        const auto exc_in_page = exc_info->ExceptionRecord->ExceptionInformation[2];
        const auto* context = exc_info->ContextRecord;
        const auto stack_trace = get_stack_trace();

        LOG_ERROR("______________________________");
        LOG_ERROR(std::format("Game crashed with code 0x%llx", exc_code).c_str());
        LOG_ERROR(std::format("Game crashed at {}", exc_address).c_str());
        switch (exc_code) {
        case EXCEPTION_ACCESS_VIOLATION:
            switch (exc_access_violation) {
            case static_cast<ULONG_PTR>(exception_access_vioaltion_types::READ_INACCESSIBLE_DATA):
                LOG_ERROR("Cause: EXCEPTION_ACCESS_VIOLATION-0");
                LOG_ERROR(std::format("Attempted to read from: 0x%llx", exc_access_address).c_str());
                break;
            case static_cast<ULONG_PTR>(exception_access_vioaltion_types::WRITE_INACCESSIBLE_DATA):
                LOG_ERROR("Cause: EXCEPTION_ACCESS_VIOLATION-1");
                LOG_ERROR(std::format("Attempted to write to: 0x%llx", exc_access_address).c_str());
                break;
            case static_cast<ULONG_PTR>(exception_access_vioaltion_types::EXECUTE_NON_EXECUTABLE_DATA):
                LOG_ERROR("Cause: EXCEPTION_ACCESS_VIOLATION-8 at 0x%llx", exc_access_address);
                break;
            default:
                LOG_ERROR("Cause: EXCEPTION_ACCESS_VIOLATION-unknown_type at 0x%llx", exc_access_address);
                break;
            }
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: LOG_ERROR("Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
        case EXCEPTION_BREAKPOINT: LOG_ERROR("Cause: EXCEPTION_BREAKPOINT"); break;
        case EXCEPTION_DATATYPE_MISALIGNMENT: LOG_ERROR("Cause: EXCEPTION_DATATYPE_MISALIGNMENT"); break;
        case EXCEPTION_FLT_DENORMAL_OPERAND: LOG_ERROR("Cause: EXCEPTION_FLT_DENORMAL_OPERAND"); break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: LOG_ERROR("Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO"); break;
        case EXCEPTION_FLT_INEXACT_RESULT: LOG_ERROR("Cause: EXCEPTION_FLT_INEXACT_RESULT"); break;
        case EXCEPTION_FLT_INVALID_OPERATION: LOG_ERROR("Cause: EXCEPTION_FLT_INVALID_OPERATION"); break;
        case EXCEPTION_FLT_OVERFLOW: LOG_ERROR("Cause: EXCEPTION_FLT_OVERFLOW"); break;
        case EXCEPTION_FLT_STACK_CHECK: LOG_ERROR("Cause: EXCEPTION_FLT_STACK_CHECK"); break;

        case EXCEPTION_FLT_UNDERFLOW: LOG_ERROR("Cause: EXCEPTION_FLT_UNDERFLOW"); break;
        case EXCEPTION_ILLEGAL_INSTRUCTION: LOG_ERROR("Cause: EXCEPTION_ILLEGAL_INSTRUCTION"); break;
        case EXCEPTION_IN_PAGE_ERROR:
            switch (exc_in_page) {
            case static_cast<unsigned int>(exception_in_page_error_types::READ_ACCESS):
                LOG_ERROR("Cause: EXCEPTION_IN_PAGE_ERROR-0");
                LOG_ERROR("Attempted to read from: 0x%llx", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::WRITE_ACCESS):
                LOG_ERROR("Cause: EXCEPTION_IN_PAGE_ERROR-1");
                LOG_ERROR("Attempted to write to: 0x%llx", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::EXECUTE_ACCESS):
                LOG_ERROR("Cause: EXCEPTION_IN_PAGE_ERROR-8 at 0x%llx", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::GUARD_PAGE):
                LOG_ERROR("Cause: EXCEPTION_IN_PAGE_ERROR-0x%llx80000001 at 0x%llx", exc_access_address);
                break;
            case static_cast<unsigned int>(exception_in_page_error_types::DEP):
                LOG_ERROR("Cause: EXCEPTION_IN_PAGE_ERROR-0x%llx80000002 at 0x%llx", exc_access_address);
                break;
            default:
                LOG_ERROR("Cause: EXCEPTION_IN_PAGE_ERROR-unknown_type at 0x%llx", exc_access_address);
                break;
            }
            break;

        case EXCEPTION_INT_DIVIDE_BY_ZERO: LOG_ERROR("Cause: EXCEPTION_INT_DIVIDE_BY_ZERO"); break;
        case EXCEPTION_INT_OVERFLOW: LOG_ERROR("Cause: EXCEPTION_INT_OVERFLOW"); break;
        case EXCEPTION_INVALID_DISPOSITION: LOG_ERROR("Cause: EXCEPTION_INVALID_DISPOSITION"); break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: LOG_ERROR("Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
        case EXCEPTION_PRIV_INSTRUCTION: LOG_ERROR("Cause: EXCEPTION_PRIV_INSTRUCTION"); break;
        case EXCEPTION_STACK_OVERFLOW: LOG_ERROR("Cause: EXCEPTION_STACK_OVERFLOW"); break;
        default: LOG_ERROR("Cause: unknown exception"); break;
        }

        LOG_ERROR("______________________________");
        LOG_ERROR("RAX: 0x%llx", context->Rax);
        LOG_ERROR("RSI: 0x%llx", context->Rsi);
        LOG_ERROR("RBX: 0x%llx", context->Rbx);
        LOG_ERROR("RDI: 0x%llx", context->Rdi);
        LOG_ERROR("RCX: 0x%llx", context->Rcx);
        LOG_ERROR("RBP: 0x%llx", context->Rbp);
        LOG_ERROR("RDX: 0x%llx", context->Rdx);
        LOG_ERROR("RSP: 0x%llx", context->Rsp);
        LOG_ERROR("R8: 0x%llx", context->R8);
        LOG_ERROR("R9: 0x%llx", context->R9);
        LOG_ERROR("R10: 0x%llx", context->R10);
        LOG_ERROR("R11: 0x%llx", context->R11);
        LOG_ERROR("R12: 0x%llx", context->R12);
        LOG_ERROR("R13: 0x%llx", context->R13);
        LOG_ERROR("R14: 0x%llx", context->R14);
        LOG_ERROR("R15: 0x%llx", context->R15);
        LOG_ERROR("______________________________");
        LOG_ERROR("stack trace: %s", stack_trace.c_str());
        LOG_ERROR("______________________________");
        system("pause");

        return EXCEPTION_CONTINUE_SEARCH;
    }
}