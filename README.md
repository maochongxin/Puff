## PuffDB

维护一个 hash 表, 存储 Index { key, file_index, file_offset, key_size, value_size } 的结构, hash 表中并不存储 value 的值, 只记录 value 的位置及长度. 



DB 接口:

```

DB::Open(const std::string& dbname, DB** db);

DB::Get(const std::string& key, std::string* result);

DB::Pus(const std::string& key, const std::string& value);

DB::Delete(const std::string& key);

DB::Recover();
```
