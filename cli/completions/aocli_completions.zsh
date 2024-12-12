#compdef aocli

_aocli() {
    local -a commands options days years parts

    commands=(
        'fetch:Fetch puzzle input'
        'view:View puzzle description'
        'submit:Submit puzzle answer'
        'update-cookie:Update session cookie'
        'cookie-status:Check cookie validity'
    )

    options=(
        '-f:Force refresh cached content'
        '--refresh:Force refresh cached content'
    )

    days=($(seq 1 25))
    years=($(seq 2015 $(date +%Y)))
    parts=('1:Part one' '2:Part two')

    _arguments -C \
        '1: :->command' \
        '2: :->first_arg' \
        '3: :->second_arg' \
        '4: :->third_arg' \
        '5: :->fourth_arg' \
        '*: :->args' \
        && return 0

    case $state in
        command)
            _describe -t commands 'commands' commands
            ;;
        first_arg)
            case $words[2] in
                fetch|view)
                    _describe -t options 'options' options
                    _describe -t days 'days' days
                    ;;
                submit)
                    _describe -t parts 'parts' parts
                    ;;
            esac
            ;;
        second_arg)
            case $words[2] in
                fetch|view)
                    _describe -t years 'years' years
                    ;;
                submit)
                    _message 'answer'
                    ;;
            esac
            ;;
        third_arg)
            case $words[2] in
                submit)
                    _describe -t days 'days' days
                    ;;
            esac
            ;;
        fourth_arg)
            case $words[2] in
                submit)
                    _describe -t years 'years' years
                    ;;
            esac
            ;;
    esac
}

_aocli
