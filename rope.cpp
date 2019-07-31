#include "rope.h"

constexpr int chunk_default_size = 256;
constexpr int chunk_min_size = 128;
constexpr int chunk_max_size = 512;

struct rope::chunk {
  std::string text;
  chunk *next = nullptr;
  chunk *prev = nullptr;
};

rope::rope(const std::string &buffer) {
  std::pair<chunk *, chunk *> list = split(buffer);
  first = list.first;
  last = list.second;
}

void rope::insert(int position, const std::string &input) {
  int chunk_total = 0;
  chunk *modify;

  for (chunk *current_chunk = first; current_chunk != nullptr;
       current_chunk = current_chunk->next) {

    if (chunk_total + current_chunk->text.size() < position) {
      chunk_total += current_chunk->text.size();
    } else {
      modify = current_chunk;
      int offset = position - chunk_total;

      current_chunk->text.insert(current_chunk->text.begin() + offset,
                                 input.begin(), input.end());

      std::pair<chunk *, chunk *> modify_location = split(current_chunk->text);

      if (current_chunk->prev != nullptr) {
        current_chunk->prev->next = modify_location.first;
      } else {
        first = modify_location.first;
      }

      if (current_chunk->next != nullptr) {
        current_chunk->next->prev = modify_location.second;
      } else {
        last = modify_location.second;
      }

      modify_location.first->prev = current_chunk->prev;
      modify_location.second->next = current_chunk->next;

      delete current_chunk;
      break;
    }
  }
}

std::pair<rope::chunk *, rope::chunk *> rope::split(std::string buffer) {
  chunk *first;
  chunk *last;

  if (buffer.size() > chunk_default_size) {
    int chunk_num = buffer.size() / chunk_default_size;

    first = new chunk;
    chunk *so_far = first;

    first->text.insert(first->text.begin(), buffer.begin(),
                       buffer.begin() + chunk_default_size);

    for (int i = 1; i < chunk_num; i++) {
      chunk *new_chunk = new chunk;
      so_far->next = new_chunk;
      new_chunk->prev = so_far;
      so_far = new_chunk;

      if (i == chunk_num - 1) {
        so_far->text.insert(so_far->text.begin(),
                            buffer.begin() + chunk_default_size * i,
                            buffer.end());
      }

      else {
        so_far->text.insert(
            so_far->text.begin(), buffer.begin() + chunk_default_size * i,
            buffer.begin() + chunk_default_size * i + chunk_default_size);
      }
    }

    last = so_far;
  } else {
    first = new chunk;
    first->text.insert(first->text.begin(), buffer.begin(), buffer.end());
    last = first;
  }

  return std::make_pair(first, last);
}
