-- Mapping helpers ==========================================================
local map = function(mode, lhs, rhs, desc, opts)
  opts = opts or {}
  opts.desc = desc
  vim.keymap.set(mode, lhs, rhs, opts)
end

local nmap = function(...) map("n", ...) end

local C = function(cmd) return "<Cmd>" .. cmd .. "<CR>" end

-- Basic mappings ===========================================================
nmap("<leader><M-w>", C "OverseerRunCmd make init", "init")
nmap("<M-w>", C "OverseerRunCmd make all", "all")
nmap("<M-r>", C "OverseerRunCmd make run", "run")
