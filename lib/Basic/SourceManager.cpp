#include "nova/Basic/SourceManager.hpp"
#include "nova/Basic/SourceLocation.hpp"
#include <cstdint>
#include <cstdio>
namespace nova {
    FileEntry::FileEntry(uint16_t id, std::string name, std::string data)
        : filename(std::move(name)), content(std::move(data)),file_id(id) {
            compute_line_offsets();
    }
    void FileEntry::compute_line_offsets(){
        line_offsets.clear();
        // first line starts at offset 0
        line_offsets.push_back(0);
        for(size_t i=0;i<content.size();++i){
            if(content[i]=='\n'){
                line_offsets.push_back(static_cast<uint32_t>(i+1));
            }
        }
    }

    uint16_t SourceManager::add_file(std::string filename, std::string content){
        uint16_t file_id =static_cast<uint16_t>(files_.size()+1);
        files_.emplace_back(std::make_unique<FileEntry>(file_id,std::move(filename),std::move(content)));
        return file_id;
    } 
    const FileEntry* SourceManager::get_file(uint16_t file_id) const{
        if(file_id==0 || file_id > files_.size()){
            return nullptr;
        } 
        //file_id is 1-based index
        //while files_ is 0-based index
        return files_[file_id-1].get();
    }
    // get char at source location from source file
    char SourceManager::get_char(SourceLocation loc) const{
        const FileEntry* file = get_file(loc.get_file_id());
        if(!file){
            return '\0';
        }
        uint32_t offset = loc.get_offset();
        if(offset >= file->content.size()){
            return '\0';
        }
        return file->content[offset];
    }
    std::string_view SourceManager::get_text(SourceRange range) const{
        const FileEntry* file = get_file(range.begin().get_file_id());
        if(!file){
            return {};
        }
        uint32_t begin_offset = range.begin().get_offset();
        uint32_t end_offset = range.end().get_offset();
        //end of string is '
        if(begin_offset >= file->content.size() || end_offset > file->content.size()|| begin_offset > end_offset){
            return {};
        } 
        return std::string_view(file->content).substr(begin_offset,end_offset - begin_offset);
    }

    void SourceManager::get_line_column(SourceLocation loc, uint32_t& line, uint32_t& column) const{
        const FileEntry* file =get_file(loc.get_file_id());
        if(!file){
            line = 0;
            column = 0;
            return;
        }
        uint32_t offset = loc.get_offset();
        //binary search for line number
        //find the largest line_offsets[i] <= offset
        //size_t is too large for comparison with uint32_t
        uint32_t left=0;
        uint32_t right = static_cast<uint32_t>(file->line_offsets.size()-1);
        int64_t pos = -1;
        while(left <= right){
            uint32_t mid = left + (right - left)/2;
            if(file->line_offsets[mid] <= offset){
                pos = mid;
                left = mid + 1;
            }else {
                right = mid -1;
            }
        }
        if(pos == -1){
            line = 0;
            column = 0;
            return;
        }
        line = static_cast<uint32_t>(pos + 1); //line number is 1-based
        column = offset - file->line_offsets[pos] + 1; //column number is 1-based
    }

    std::string_view SourceManager::get_filename(SourceLocation loc) const{
        const FileEntry* file =get_file(loc.get_file_id());
        if(!file){
            return {};
        }
        return file->filename;
    }
    //format source location as "filename:line:column"
    std::string SourceManager::format_location(SourceLocation loc) const{
        const FileEntry* file =get_file(loc.get_file_id());
        if(!file){
            return "<invalid location>";
        }
        uint32_t line,column;
        get_line_column(loc, line, column);
        char buffer[1024];
        std::snprintf(buffer,sizeof(buffer),"%s:%u:%u",file->filename.c_str(),line,column);
        return std::string(buffer);
    }
}