# Commands
complete -c aocli -f
complete -c aocli -n "__fish_use_subcommand" -a fetch -d "Fetch puzzle input"
complete -c aocli -n "__fish_use_subcommand" -a view -d "View puzzle description"
complete -c aocli -n "__fish_use_subcommand" -a submit -d "Submit puzzle answer"
complete -c aocli -n "__fish_use_subcommand" -a update-cookie -d "Update session cookie"
complete -c aocli -n "__fish_use_subcommand" -a cookie-status -d "Check cookie validity"

# Options
complete -c aocli -n "__fish_seen_subcommand_from fetch view" -s f -l refresh -d "Force refresh cached content"

# Days (1-25)
complete -c aocli -n "__fish_seen_subcommand_from fetch view" -a "(seq 1 25)" -d "Day"

# Years (2015-current)
complete -c aocli -n "__fish_seen_subcommand_from fetch view" -a "(seq 2015 (date +%Y))" -d "Year"

# Submit command completions
complete -c aocli -n "__fish_seen_subcommand_from submit; and not __fish_seen_argument -l part" -a "1 2" -d "Part"
complete -c aocli -n "__fish_seen_subcommand_from submit; and __fish_seen_argument -l part" -a "(seq 1 25)" -d "Day"
