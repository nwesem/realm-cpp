////////////////////////////////////////////////////////////////////////////
//
// Copyright 2022 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#ifndef realm_thread_safe_reference_hpp
#define realm_thread_safe_reference_hpp

#include <future>
#include <queue>
#include <iostream>

#include <cpprealm/internal/bridge/obj.hpp>
#include <cpprealm/internal/bridge/thread_safe_reference.hpp>

namespace realm {
    template <typename>
    struct object;
    class ThreadSafeReference;
    class Realm;
template <typename ...>
struct db;

template <typename T, typename = void>
struct thread_safe_reference;

template <typename T>
struct thread_safe_reference<T, std::enable_if_t<std::is_base_of_v<object<T>, T>>> {
    explicit thread_safe_reference(const T& object)
    : m_tsr(internal::bridge::thread_safe_reference(*object.m_object))
    {
    }
    thread_safe_reference(internal::bridge::thread_safe_reference&& tsr)
    : m_tsr(std::move(tsr))
    {
    }
    T resolve(std::shared_ptr<Realm> const&);
private:
    internal::bridge::thread_safe_reference m_tsr;
    template <typename ...Ts>
    friend struct db;
};

template <typename ...Ts>
struct thread_safe_reference<db<Ts...>> {
    thread_safe_reference(internal::bridge::thread_safe_reference&& tsr)
            : m_tsr(std::move(tsr))
    {
    }
    thread_safe_reference() = default;
    thread_safe_reference(const thread_safe_reference&) = default;
    thread_safe_reference(thread_safe_reference&&) = default;
    thread_safe_reference& operator=(const thread_safe_reference&) = default;
    thread_safe_reference& operator=(thread_safe_reference&&) noexcept = default;
    ~thread_safe_reference() = default;

    db<Ts...> resolve()
    {
        return db<Ts...>(internal::bridge::realm(std::move(m_tsr)));
    }

    explicit operator bool() const noexcept
    {
        return m_tsr.operator bool();
    }
private:
    internal::bridge::thread_safe_reference m_tsr;
    template <typename ...>
    friend struct db;
};

// TODO: Renable async_open with task.
#if __cpp_coroutines
    //template <typename ...Ts>
//static inline task<thread_safe_reference<db<Ts...>>> async_open(const db_config& config) {
//    // TODO: Add these flags to core
//#if QT_CORE_LIB
//    util::Scheduler::set_default_factory(util::make_qt);
//#endif
//    std::vector<ObjectSchema> schema;
//    (schema.push_back(Ts::schema::to_core_schema()), ...);
//
//    auto tsr = co_await make_awaitable<ThreadSafeReference>([config, schema](auto cb) {
//        std::shared_ptr<AsyncOpenTask> async_open_task = Realm::get_synchronized_realm({
//                                                                                               .path = config.path,
//                                                                                               .schema_mode = SchemaMode::AdditiveExplicit,
//                                                                                               .schema = Schema(schema),
//                                                                                               .schema_version = 0,
//                                                                                               .sync_config = config.sync_config
//                                                                                       });
//        async_open_task->start(cb);
//    });
//    co_return thread_safe_reference<db<Ts...>>(std::move(tsr));
//}
#else

#endif
}


#endif /* realm_thread_safe_reference.hpp */
