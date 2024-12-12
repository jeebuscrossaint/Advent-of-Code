_aocli_complete() {
    local cur prev words cword
    _init_completion || return

    # List of all commands
    local commands="fetch view submit update-cookie cookie-status"

    # List of options/flags
    local options="-f --refresh"

    case $prev in
        aocli)
            # Complete commands
            COMPREPLY=($(compgen -W "$commands" -- "$cur"))
            return
            ;;
        fetch|view)
            # Complete with numbers 1-25 for day
            if [[ $cur =~ ^[0-9]*$ ]]; then
                COMPREPLY=($(compgen -W "$(seq 1 25)" -- "$cur"))
            fi
            return
            ;;
        submit)
            # Complete part number (1 or 2)
            if [[ $cword -eq 2 ]]; then
                COMPREPLY=($(compgen -W "1 2" -- "$cur"))
            # Complete day number after answer
            elif [[ $cword -eq 4 ]]; then
                COMPREPLY=($(compgen -W "$(seq 1 25)" -- "$cur"))
            fi
            return
            ;;
    esac

    # Complete options for commands that accept them
    if [[ $cur == -* ]]; then
        COMPREPLY=($(compgen -W "$options" -- "$cur"))
        return
    fi
}

complete -F _aocli_complete aocli
