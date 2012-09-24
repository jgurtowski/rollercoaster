#include <deque>
#include <cassert>
#include <iostream>

#include "list_backed_kmer.h"


namespace rollercoaster{

    void ListBackedKmer::str_kmer(std::string &out) const{
        out.reserve(kmer_size_);
        out.clear();
        std::copy(list_kmer_.begin(), list_kmer_.end(), std::back_inserter(out));
    }

    std::string ListBackedKmer::str_kmer() const{
        std::string t;
        str_kmer(t);
        return t;
    }

    std::ostream& operator<<(std::ostream& stream, const ListBackedKmer& lhs)
    {
        stream << '[';
        for (std::deque<char>::const_iterator it = lhs.begin();
                it != lhs.end(); ++it) {
            stream << ' ' << *it;
        }
        stream << " ]";
        return stream;
    }

    int compare(const ListBackedKmer &lhs, const ListBackedKmer &rhs){
        //std::cout << "lhs: " << lhs;
        //std::cout << "\nrhs: " << rhs << std::endl;
        assert(lhs.list_kmer_.size() == rhs.list_kmer_.size());
        std::deque<char>::const_iterator lbegin = lhs.list_kmer_.begin(),
            lend = lhs.list_kmer_.end(),
            rbegin = rhs.list_kmer_.begin();

        for(;lbegin != lend; ++lbegin, ++rbegin){
            if(*lbegin < *rbegin)
                return -1;
            else if(*lbegin > *rbegin)
                return 1;
        }
        return 0;
    }

}//namespace rollercoaster

