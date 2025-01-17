#include <cpprealm/internal/bridge/mixed.hpp>
#include <cpprealm/internal/bridge/utils.hpp>

#include <realm/mixed.hpp>
//#include <cpprealm/object.hpp>

//#include <cpprealm/internal/bridge/obj.hpp>
//#include <cpprealm/internal/bridge/realm.hpp>
//#include <cpprealm/internal/bridge/schema.hpp>

namespace realm::internal::bridge {
#ifdef __i386__
    static_assert(SizeCheck<20, sizeof(Mixed)>{});
    static_assert(SizeCheck<4, alignof(Mixed)>{});
#elif __x86_64__
    static_assert(SizeCheck<24, sizeof(Mixed)>{});
    static_assert(SizeCheck<8, alignof(Mixed)>{});
#elif __arm__
    static_assert(SizeCheck<24, sizeof(Mixed)>{});
    static_assert(SizeCheck<8, alignof(Mixed)>{});
#elif __aarch64__
    static_assert(SizeCheck<24, sizeof(Mixed)>{});
    static_assert(SizeCheck<8, alignof(Mixed)>{});
#endif

#define CPPREALM_OPTIONAL_MIXED(type) \
    template<> \
    mixed::mixed(const std::optional<type>& o) { \
        if (o) { \
            *this = mixed(*o); \
        } else { \
            new (&m_mixed) Mixed(realm::none); \
        } \
    } \

    mixed::mixed(const std::monostate&) {
        new (&m_mixed) Mixed(std::nullopt);
    }

    mixed::mixed(const std::string &v) {
        m_owned_string = v;
        new (&m_mixed) Mixed(v);
    }
    mixed::mixed(const timestamp &v) {
        new (&m_mixed) Mixed(v);
    }
    mixed::mixed(const int &v) {
        new (&m_mixed) Mixed(v);
    }
    mixed::mixed(const int64_t &v) {
        new (&m_mixed) Mixed(v);
    }
    mixed::mixed(const double &v) {
        new (&m_mixed) Mixed(v);
    }
    mixed::mixed(const struct uuid &v) {
        new (&m_mixed) Mixed(static_cast<UUID>(v));
    }
    mixed::mixed(const struct object_id &v) {
        new (&m_mixed) Mixed(static_cast<ObjectId>(v));
    }
    mixed::mixed(const realm::Mixed &v) {
        if (v.is_null()) {
            new (&m_mixed) Mixed();
        } else {
            if (v.get_type() == type_String) {
                m_owned_string = v.get_string();
            }
            new (&m_mixed) Mixed(v);
        }
    }
    mixed::mixed(const struct binary &v) {
        new (&m_mixed) Mixed(static_cast<BinaryData>(v));
    }
    mixed::mixed(const struct obj_link &v) {
        new (&m_mixed) Mixed(static_cast<ObjLink>(v));
    }
    mixed::mixed(const struct obj_key &v) {
        new (&m_mixed) Mixed(static_cast<ObjKey>(v));
    }
    mixed::mixed(const bool &v) {
        new (&m_mixed) Mixed(v);
    }

    CPPREALM_OPTIONAL_MIXED(std::string);
    CPPREALM_OPTIONAL_MIXED(timestamp);
    CPPREALM_OPTIONAL_MIXED(int);
    CPPREALM_OPTIONAL_MIXED(int64_t);
    CPPREALM_OPTIONAL_MIXED(double);
    CPPREALM_OPTIONAL_MIXED(struct uuid);
    CPPREALM_OPTIONAL_MIXED(struct object_id);
    CPPREALM_OPTIONAL_MIXED(struct binary);
    CPPREALM_OPTIONAL_MIXED(struct obj_link);
    CPPREALM_OPTIONAL_MIXED(struct obj_key);
    CPPREALM_OPTIONAL_MIXED(bool);

    mixed::operator Mixed() const {
        if (!is_null() && type() == data_type::String) {
            return m_owned_string;
        }
        return *reinterpret_cast<const Mixed*>(m_mixed);
    }

    mixed::operator std::string() const {
        return m_owned_string;
    }
    mixed::operator bridge::binary() const {
        return reinterpret_cast<const Mixed*>(m_mixed)->get_binary();
    }
    mixed::operator bridge::timestamp() const {
        return reinterpret_cast<const Mixed*>(m_mixed)->get_timestamp();
    }
    mixed::operator bridge::obj_link() const {
        return reinterpret_cast<const Mixed*>(m_mixed)->get<ObjLink>();
    }
    mixed::operator bridge::obj_key() const {
        return reinterpret_cast<const Mixed*>(m_mixed)->get<ObjKey>();
    }
    mixed::operator bridge::uuid() const {
        return static_cast<const uuid &>(reinterpret_cast<const Mixed *>(m_mixed)->get_uuid());
    }
    mixed::operator bridge::object_id() const {
        return static_cast<const object_id &>(reinterpret_cast<const Mixed *>(m_mixed)->get_object_id());
    }
    mixed::operator int64_t() const {
        return static_cast<const int64_t &>(reinterpret_cast<const Mixed *>(m_mixed)->get_int());
    }
    mixed::operator double() const {
        return static_cast<const double &>(reinterpret_cast<const Mixed *>(m_mixed)->get_double());
    }
    mixed::operator bool() const {
        return static_cast<const bool &>(reinterpret_cast<const Mixed *>(m_mixed)->get_bool());
    }
    data_type mixed::type() const noexcept {
        return data_type(static_cast<int>(reinterpret_cast<const Mixed *>(m_mixed)->get_type()));
    }
    bool mixed::is_null() const noexcept {
        return reinterpret_cast<const Mixed *>(m_mixed)->is_null();
    }
#define __cpp_realm_gen_mixed_op(op) \
    bool operator op(const mixed& a, const mixed& b) { \
        return a.operator Mixed() op b.operator Mixed();                                       \
    }

    __cpp_realm_gen_mixed_op(==)
    __cpp_realm_gen_mixed_op(!=)
    __cpp_realm_gen_mixed_op(>)
    __cpp_realm_gen_mixed_op(<)
    __cpp_realm_gen_mixed_op(>=)
    __cpp_realm_gen_mixed_op(<=)
}