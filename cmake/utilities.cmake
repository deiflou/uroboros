function(uroboros_info_message)
    set(
        msg
        "***********************************************************************\n"
        "* UROBOROS MESSAGE:\n"
    )

    foreach(arg ${ARGV})
        set(msg ${msg} "*   ${arg}\n")
    endforeach()
    
    set(
        msg
        ${msg}
        "***********************************************************************"
    )
    
    message(STATUS ${msg})
endfunction()

function(uroboros_notify_example_will_not_be_built example_name reason)
    uroboros_info_message(
        "The example named \"${example_name}\" will not be built."
        "Reason: ${reason}."
    )
endfunction()
