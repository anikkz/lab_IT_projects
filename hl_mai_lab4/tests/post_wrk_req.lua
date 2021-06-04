counter = 0

wrk.method = "POST"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"

request = function()
   path = "/person"
   wrk.body = "last_name=Nikitina&first_name=Anna&age=24&login=anikkz" .. counter
   counter = counter + 1
   return wrk.format(nil, path)
end
