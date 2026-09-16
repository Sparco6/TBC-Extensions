MWDT = MWDT or {}
MWDT.version = "1.2.0"
MWDT.modules = MWDT.modules or {}
MWDT.tabs = MWDT.tabs or {}
MWDT.events = MWDT.events or {}
MWDT.colors = { title="|cff33ff99", info="|cff66ccff", warn="|cffffcc00", err="|cffff5555", dim="|cffaaaaaa", reset="|r" }

local function chat(msg)
    if DEFAULT_CHAT_FRAME then DEFAULT_CHAT_FRAME:AddMessage("|cff33ff99MWDT:|r "..tostring(msg)) end
end
MWDT.Print = chat

function MWDT.SafeCall(fn, ...)
    if type(fn) ~= "function" then return false, "not a function" end
    local ok, a,b,c,d,e,f,g,h = pcall(fn, ...)
    if not ok then return false, a end
    return true, a,b,c,d,e,f,g,h
end

function MWDT.SerializeValue(v, depth, seen)
    depth = depth or 0
    seen = seen or {}
    local t = type(v)
    if t == "string" then return string.format("%q", v) end
    if t == "number" or t == "boolean" or t == "nil" then return tostring(v) end
    if t == "function" then return "<function>" end
    if t == "userdata" then return "<userdata:"..tostring(v)..">" end
    if t == "thread" then return "<thread>" end
    if t ~= "table" then return "<"..t..">" end
    if seen[v] then return "<cycle>" end
    if depth >= 2 then return "{...}" end
    seen[v] = true
    local out, n = {}, 0
    for k,val in pairs(v) do
        n=n+1; if n>30 then out[#out+1]="..."; break end
        out[#out+1] = "["..MWDT.SerializeValue(k,depth+1,seen).."]="..MWDT.SerializeValue(val,depth+1,seen)
    end
    seen[v]=nil
    return "{"..table.concat(out,", ").."}"
end

function MWDT.Eval(code)
    if type(loadstring) ~= "function" then return false, "loadstring is unavailable in this client" end
    local fn, err = loadstring(code)
    if not fn then return false, err end
    return MWDT.SafeCall(fn)
end

function MWDT.RegisterModule(name, module)
    MWDT.modules[name] = module
end

function MWDT.RegisterTab(id, title, builder)
    MWDT.tabs[#MWDT.tabs+1] = { id=id, title=title, builder=builder }
end

function MWDT.TableCount(t)
    local n=0; for _ in pairs(t or {}) do n=n+1 end; return n
end

function MWDT.Trim(s)
    return (string.gsub(string.gsub(tostring(s or ""), "^%s+", ""), "%s+$", ""))
end

function MWDT.SplitWords(s)
    local out={}; for w in string.gmatch(tostring(s or ""), "%S+") do out[#out+1]=w end; return out
end

function MWDT.CopyTextToChatBox(text)
    if ChatFrameEditBox then
        ChatFrameEditBox:Show(); ChatFrameEditBox:SetFocus(); ChatFrameEditBox:SetText(tostring(text or "")); ChatFrameEditBox:HighlightText()
    else chat(text) end
end

local f = CreateFrame("Frame")
f:RegisterEvent("ADDON_LOADED")
f:RegisterEvent("PLAYER_LOGIN")
f:SetScript("OnEvent", function()
    if event == "ADDON_LOADED" and arg1 == "MasterWoWDevTools" then
        MasterWoWDevToolsDB = MasterWoWDevToolsDB or {}
        MasterWoWDevToolsDB.savedScripts = MasterWoWDevToolsDB.savedScripts or {}
        MasterWoWDevToolsDB.history = MasterWoWDevToolsDB.history or {}
        MasterWoWDevToolsDB.watch = MasterWoWDevToolsDB.watch or {}
        MasterWoWDevToolsDB.researchNotes = MasterWoWDevToolsDB.researchNotes or {}
        MasterWoWDevToolsDB.runtimeSnapshots = MasterWoWDevToolsDB.runtimeSnapshots or {}
        MasterWoWDevToolsDB.settings = MasterWoWDevToolsDB.settings or { eventLimit=400, outputLimit=600 }
    elseif event == "PLAYER_LOGIN" then
        chat("loaded v"..MWDT.version..". Use /mdev or /devtools")
    end
end)

SLASH_MWDT1 = "/mdev"
SLASH_MWDT2 = "/devtools"
SlashCmdList["MWDT"] = function(msg)
    msg = MWDT.Trim(msg)
    if msg == "" then MWDT.Toggle() return end
    local cmd, rest = string.match(msg, "^(%S+)%s*(.*)$")
    cmd = string.lower(cmd or "")
    if cmd == "run" then
        local ok,a,b,c = MWDT.Eval(rest)
        if ok then chat("=> "..MWDT.SerializeValue(a).." "..MWDT.SerializeValue(b).." "..MWDT.SerializeValue(c)) else chat(MWDT.colors.err..tostring(a)..MWDT.colors.reset) end
    elseif cmd == "api" or cmd == "globals" then MWDT.ShowTab("browser")
    elseif cmd == "cvar" then MWDT.ShowTab("cvar")
    elseif cmd == "frame" then MWDT.ShowTab("frames")
    elseif cmd == "event" then MWDT.ShowTab("events")
    elseif cmd == "item" then MWDT.ShowTab("item"); if MWDT.ItemInspectByID then MWDT.ItemInspectByID(tonumber(rest)) end
    elseif cmd == "spell" then MWDT.ShowTab("spell"); if MWDT.SpellInspectByID then MWDT.SpellInspectByID(tonumber(rest)) end
    elseif cmd == "cap" then MWDT.ShowTab("capabilities")
    elseif cmd == "research" then MWDT.ShowTab("research")
    elseif cmd == "model" then MWDT.ShowTab("models")
    else
        chat("Commands: /mdev, /mdev run <lua>, api, cvar, frame, event, item <id>, spell <id>, cap, research, model")
    end
end
