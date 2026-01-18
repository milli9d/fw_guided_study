#include <cerrno>
#include <memory>
#include <print>

template<typename T>
class forward_list {
  public:
    class node_t {
      public:
        T val;
        std::shared_ptr<node_t> next;

      public:
        node_t(const T& val, std::shared_ptr<node_t> next = nullptr) : val(val), next(next) {}
    };

  private:
    std::shared_ptr<node_t> _head{};
    size_t _sz{};

  public:
    void print() {
        if (_sz == 0u) {
            return;
        }
        std::shared_ptr<node_t> ptr = _head;
        std::print("Size {} | HEAD -> ", _sz);
        while (ptr != nullptr) {
            std::print("[{}] -> ", ptr->val);
            ptr = ptr->next;
        }
        std::print("END\n");
    }

    const T& front() {
        return _head->val;
    }

    size_t size() {
        return _sz;
    }

    bool empty() {
        return _sz == 0u;
    }

    void append(const T& val) {
        _head = std::make_shared<node_t>(val, _head);
        _sz++;
    }

    inline void push_front(const T& val) {
        append(val);
    }

    void push_back(const T& val) {
        std::shared_ptr<node_t> ptr = _head;
        while (ptr && ptr->next) { ptr = ptr->next; }
        ptr->next = std::make_shared<node_t>(val);
        _sz++;
    }

    int32_t insert(const T& val, size_t idx) {
        if (idx > _sz) {
            return -ENODATA;
        }

        /* handle special cases */
        if (idx == _sz) {
            push_back(val);
            return 0;
        }
        if (idx == 0u) {
            append(val);
            return 0;
        }

        /* find idx and insert */
        std::shared_ptr<node_t> ptr = _head;
        while (ptr && idx-- > 1u) { ptr = ptr->next; }
        ptr->next = std::make_shared<node_t>(val, ptr->next);
        _sz++;

        return 0;
    }

    void reverse() {
        std::shared_ptr<node_t> curr = _head;
        std::shared_ptr<node_t> prev = nullptr;
        while (curr) {
            std::shared_ptr<node_t> link_fw = curr->next;
            curr->next = prev;
            prev = curr;
            curr = link_fw;
        }
        _head = prev;
    }

    void pop_front() {
        if (_head) {
            _head = _head->next;
            _sz--;
        }
    }

    int32_t remove(size_t idx) {
        if (idx >= _sz) {
            return -ENODATA;
        }

        /* handle special cases */
        if (idx == 0) {
            pop_front();
            return 0;
        }

        /* find index and remove */
        std::shared_ptr<node_t> ptr = _head;
        while (ptr && idx-- > 1u) { ptr = ptr->next; }
        ptr->next = ptr->next->next;
        _sz--;

        return 0;
    }
};