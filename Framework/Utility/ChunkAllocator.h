#pragma once
#include <vector>

class IChunkAllocator
{
public:
	IChunkAllocator() = default;
	virtual ~IChunkAllocator() = default;
};

template <class T, size_t CHUNKSIZE = 128>
class ChunkAllocator : public IChunkAllocator
{
	//Memory chunk=========================================================
	struct Chunk
	{
	public:
		Chunk() : numAllocated(0), memory{}
		{
			for (size_t i = 0; i < CHUNKSIZE; i++)
				handles[i] = reinterpret_cast<T*>(memory) + i;
		}
		T* begin() { return reinterpret_cast<T*>(memory); }

	public:
		size_t numAllocated;
		T*	   handles[CHUNKSIZE];
		__int8 memory[CHUNKSIZE * sizeof(T)];
	};

	using Chunks = std::vector<Chunk*>;

public:
	//Iterator for objects allocated in memory chunks======================
	class iterator
	{
	public:
		iterator(typename Chunks::iterator chunk) :
			m_chunk(chunk), m_index(0) {}
		~iterator() = default;

		inline iterator& operator++(int) {
			//Increment handle index
			m_index++;
			//If handle index reached end of chunk, go to the start of next chunk
			if (m_index >= (*m_chunk)->numAllocated) {
				m_chunk++;
				m_index = 0;
			}
			return *this;
		}
		inline T& operator*() { return (*m_chunk)->handles[m_index]; }
		inline T* operator->() { return (*m_chunk)->handles[m_index]; }
		inline bool operator==(const iterator& other) { return (m_index == other.m_index) && (m_chunk == other.m_chunk); }
		inline bool operator!=(const iterator& other) { return (m_index != other.m_index) || (m_chunk != other.m_chunk); }

	private:
		typename size_t m_index;
		typename Chunks::iterator m_chunk;
	};
	iterator begin() { return iterator(m_chunks.begin()); }
	iterator end() { return iterator(m_chunks.end()); }

public:
	//Chunk allocator=====================================================
	ChunkAllocator()
	{
		//Add initial chunk
		m_chunks.emplace_back(new Chunk);
	}
	virtual ~ChunkAllocator()
	{
		//Iterate and delete all chunks
		for (auto chunk : m_chunks) 
			delete chunk;
	}

	T* Allocate()
	{
		//If chunk with available space is found, allocate and return object
		for (auto chunk : m_chunks)
		{
			if (chunk->numAllocated >= CHUNKSIZE) continue;
			return chunk->handles[chunk->numAllocated++];
		}
		
		//If no avaiable chunk is found, add new chunk, allocate and return object
		auto chunk = m_chunks.emplace_back(new Chunk);
		return chunk->handles[chunk->numAllocated++];
	}

	void Deallocate(T* object)
	{
		//Iterate chunks until chunk that holds the object is found
		for (auto chunk : m_chunks)
			if (object >= chunk->begin() && object <= chunk->begin() + CHUNKSIZE)
				//Iterate objects in the chunk until matching object is found
				for (size_t i = 0; i < chunk->numAllocated; i++)
					if (chunk->handles[i] == object)
					{
						//Free object address
						std::swap(chunk->handles[i], chunk->handles[--chunk->numAllocated]);
						return;
					}
	}

private:
	Chunks m_chunks;
};