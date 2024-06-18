# Bash completion script for the cross_platform_fs_utility

_file_flex() {
    local cur prev commands
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    commands="create_tree compare_directories encrypt_file decrypt_file batch_rename_prefix batch_rename_postfix delete_all sync_directories find_duplicates create_sequence run_sequence search_files calculate_directory_size help"

    if [[ ${COMP_CWORD} == 1 ]]; then
        COMPREPLY=( $(compgen -W "${commands}" -- ${cur}) )
    else
        case "${prev}" in
            create_tree|compare_directories|encrypt_file|decrypt_file|batch_rename_prefix|batch_rename_postfix|delete_all|sync_directories|find_duplicates|run_sequence|search_files|calculate_directory_size)
                COMPREPLY=( $(compgen -f -- ${cur}) )
                ;;
            *)
                COMPREPLY=()
                ;;
        esac
    fi

    return 0
}

complete -F _file_flex file_flex
