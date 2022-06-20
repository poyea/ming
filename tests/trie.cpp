#include "gtest/gtest.h"

#include <ming/trie.hpp>

namespace trie_test {

class Trie_TEST : public ::testing::Test {
protected:
  Trie_TEST() = default;

  virtual ~Trie_TEST() {}

  virtual void SetUp() {
    // post-construction
  }

  virtual void TearDown() {
    // pre-destruction
  }

  ming::Trie<char> trie;
};

TEST_F(Trie_TEST, TrieSearch) {
  EXPECT_FALSE(trie.is_word("word"));
  EXPECT_FALSE(trie.starts_with("word"));
  trie.insert("word");
  EXPECT_TRUE(trie.is_word("word"));
  EXPECT_FALSE(trie.is_word("word_not_exist"));
  EXPECT_FALSE(trie.is_word("not_exist"));
}

TEST_F(Trie_TEST, TriePrefix) {
  trie.insert("word");
  EXPECT_TRUE(trie.starts_with("wo"));
  EXPECT_FALSE(trie.starts_with("po"));
}

TEST_F(Trie_TEST, TrieClone) {
  trie.insert("word");
  auto t_clone = trie;
  EXPECT_TRUE(trie.starts_with("wo"));
  EXPECT_FALSE(trie.starts_with("po"));
  EXPECT_TRUE(t_clone.starts_with("wo"));
  EXPECT_FALSE(t_clone.starts_with("po"));
}

TEST_F(Trie_TEST, TrieMoved) {
  trie.insert("word");
  auto t_moved = std::move(trie);
  auto t_copied(t_moved);
  EXPECT_TRUE(t_moved.starts_with("wo"));
  EXPECT_FALSE(t_moved.starts_with("po"));
  EXPECT_TRUE(t_copied.starts_with("wo"));
  EXPECT_FALSE(t_copied.starts_with("po"));
}

} // namespace trie_test