# metahash-btc-client

### Dependices:
```
boost 1.67
rapidjson
```

### Using:
```
run --help what would see allowed options
run --request what would will see description for requests
```

### Requests:
```
Generate wallet 
{"id":decimal, "version":"2.0","method":"generate", "params":{"password":str}}

Balance of wallet 
{"id":decimal, "version":"2.0","method":"fetch-balance", "params":{"address":str}}

History of wallet 
{"id":decimal, "version":"2.0","method":"fetch-history", "params":{"address":str}}

Create transaction 
{"id":decimal, "version":"2.0","method":"create-tx", "params":{"address":str, "password":str, "to":str, "value":decimal/all, "fee":decimal/auto, "nonce":decimal}}

Send transaction 
{"id":decimal, "version":"2.0","method":"send-tx", "params":{"address":str, "password":str, "to":str, "value":decimal/all, "fee":decimal/auto, "nonce":decimal}}
```
