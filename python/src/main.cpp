#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include <ripple/app/tx/impl/Transactor.h>
#include <ripple/protocol/st.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(plugin_transactor, m) {
    py::enum_<ripple::TxType>(m, "TxType")
        .value("ttPAYMENT", ripple::TxType::ttPAYMENT)
        .value("ttESCROW_CREATE", ripple::TxType::ttESCROW_CREATE)
        .value("ttESCROW_FINISH", ripple::TxType::ttESCROW_FINISH)
        .value("ttACCOUNT_SET", ripple::TxType::ttACCOUNT_SET)
        .value("ttESCROW_CANCEL", ripple::TxType::ttESCROW_CANCEL)
        .value("ttREGULAR_KEY_SET", ripple::TxType::ttREGULAR_KEY_SET)
        .value("ttOFFER_CREATE", ripple::TxType::ttOFFER_CREATE)
        .value("ttOFFER_CANCEL", ripple::TxType::ttOFFER_CANCEL)
        .value("ttTICKET_CREATE", ripple::TxType::ttTICKET_CREATE)
        .value("ttSIGNER_LIST_SET", ripple::TxType::ttSIGNER_LIST_SET)
        .value("ttPAYCHAN_CREATE", ripple::TxType::ttPAYCHAN_CREATE)
        .value("ttPAYCHAN_FUND", ripple::TxType::ttPAYCHAN_FUND)
        .value("ttPAYCHAN_CLAIM", ripple::TxType::ttPAYCHAN_CLAIM)
        .value("ttCHECK_CREATE", ripple::TxType::ttCHECK_CREATE)
        .value("ttCHECK_CASH", ripple::TxType::ttCHECK_CASH)
        .value("ttCHECK_CANCEL", ripple::TxType::ttCHECK_CANCEL)
        .value("ttDEPOSIT_PREAUTH", ripple::TxType::ttDEPOSIT_PREAUTH)
        .value("ttTRUST_SET", ripple::TxType::ttTRUST_SET)
        .value("ttACCOUNT_DELETE", ripple::TxType::ttACCOUNT_DELETE)
        .value("ttHOOK_SET", ripple::TxType::ttHOOK_SET)
        .value("ttNFTOKEN_MINT", ripple::TxType::ttNFTOKEN_MINT)
        .value("ttNFTOKEN_BURN", ripple::TxType::ttNFTOKEN_BURN)
        .value("ttNFTOKEN_CREATE_OFFER", ripple::TxType::ttNFTOKEN_CREATE_OFFER)
        .value("ttNFTOKEN_CANCEL_OFFER", ripple::TxType::ttNFTOKEN_CANCEL_OFFER)
        .value("ttNFTOKEN_ACCEPT_OFFER", ripple::TxType::ttNFTOKEN_ACCEPT_OFFER)
        .value("ttDUMMY_TX", ripple::TxType::ttDUMMY_TX)
        .value("ttAMENDMENT", ripple::TxType::ttAMENDMENT)
        .value("ttFEE", ripple::TxType::ttFEE)
        .value("ttUNL_MODIFY", ripple::TxType::ttUNL_MODIFY)
        .export_values();
    
    py::enum_<ripple::LedgerEntryType>(m, "LedgerEntryType")
        .value("ltACCOUNT_ROOT", ripple::LedgerEntryType::ltACCOUNT_ROOT)
        .value("ltDIR_NODE", ripple::LedgerEntryType::ltDIR_NODE)
        .value("ltRIPPLE_STATE", ripple::LedgerEntryType::ltRIPPLE_STATE)
        .value("ltTICKET", ripple::LedgerEntryType::ltTICKET)
        .value("ltSIGNER_LIST", ripple::LedgerEntryType::ltSIGNER_LIST)
        .value("ltOFFER", ripple::LedgerEntryType::ltOFFER)
        .value("ltLEDGER_HASHES", ripple::LedgerEntryType::ltLEDGER_HASHES)
        .value("ltAMENDMENTS", ripple::LedgerEntryType::ltAMENDMENTS)
        .value("ltFEE_SETTINGS", ripple::LedgerEntryType::ltFEE_SETTINGS)
        .value("ltESCROW", ripple::LedgerEntryType::ltESCROW)
        .value("ltPAYCHAN", ripple::LedgerEntryType::ltPAYCHAN)
        .value("ltCHECK", ripple::LedgerEntryType::ltCHECK)
        .value("ltDEPOSIT_PREAUTH", ripple::LedgerEntryType::ltDEPOSIT_PREAUTH)
        .value("ltNEGATIVE_UNL", ripple::LedgerEntryType::ltNEGATIVE_UNL)
        .value("ltNFTOKEN_PAGE", ripple::LedgerEntryType::ltNFTOKEN_PAGE)
        .value("ltNFTOKEN_OFFER", ripple::LedgerEntryType::ltNFTOKEN_OFFER)
        .value("ltANY", ripple::LedgerEntryType::ltANY)
        .value("ltCHILD", ripple::LedgerEntryType::ltCHILD)
        .export_values();

    py::class_<ripple::SField> SField(m, "SField");

    py::class_<ripple::STAccount> STAccount(m, "STAccount");

    py::class_<ripple::AccountID> AccountID(m, "AccountID");
    AccountID
        .def("toBase58",
            [](const ripple::AccountID &a) {
                return ripple::toBase58(a);
            }
        )
        .def("__repr__",
            [](const ripple::AccountID &a) {
                return "<AccountID:'" + ripple::toBase58(a) + "'>";
            }
        );

    py::class_<ripple::STObject, std::shared_ptr<ripple::STObject>> STObject(m, "STObject");
    STObject
        .def_property_readonly("Account",
            [](const ripple::STObject &obj) {
                return obj[ripple::sfAccount];
            }
        )
        .def("setRegularKey",
            [](ripple::STObject &obj) {
                auto const accountID = obj[ripple::sfAccount];
                obj.setAccountID(ripple::sfRegularKey, accountID);
            }
        )
        .def("__repr__",
            [](const ripple::STObject &obj) {
                return obj.getFullText();
            }
        );

    py::class_<ripple::STTx, ripple::STObject, std::shared_ptr<ripple::STTx>> STTx(m, "STTx");
    STTx
        .def("getTxnType", &ripple::STTx::getTxnType);

    py::class_<ripple::STLedgerEntry, ripple::STObject, std::shared_ptr<ripple::STLedgerEntry>> STLedgerEntry(m, "STLedgerEntry");

    py::class_<ripple::Rules> Rules(m, "Rules");
    Rules
        .def("enabled", &ripple::Rules::enabled);
    
    py::class_<ripple::PreflightContext> PreflightContext(m, "PreflightContext");
    PreflightContext
        .def_property_readonly("tx",
            [](const ripple::PreflightContext &ctx) {
                return ctx.tx;
            }
        )
        .def_property_readonly("rules",
            [](const ripple::PreflightContext &ctx) {
                return ctx.rules;
            }
        );
    
    py::class_<ripple::Keylet> Keylet(m, "Keylet");
    Keylet
        .def_property_readonly("type",
            [](const ripple::Keylet &keylet) {
                return keylet.type;
            }
        )
        .def_property_readonly("key",
            [](const ripple::Keylet &keylet) {
                return keylet.key.data();
            }
        );
    
    py::class_<ripple::ApplyView> ApplyView(m, "ApplyView");
    ApplyView
        .def("peek", &ripple::ApplyView::peek)
        .def("update", &ripple::ApplyView::update);
    
    py::class_<
        ripple::detail::ApplyViewBase,
        ripple::ApplyView
    > ApplyViewBase(m, "ApplyViewBase");
    py::class_<
        ripple::ApplyViewImpl,
        ripple::detail::ApplyViewBase
    > ApplyViewImpl(m, "ApplyViewImpl");

    py::class_<ripple::ApplyContext> ApplyContext(m, "ApplyContext");
    ApplyContext
        .def_property_readonly("tx",
            [](const ripple::ApplyContext &ctx) {
                return ctx.tx;
            }
        )
        .def("view", &ripple::ApplyContext::view, py::return_value_policy::reference);
    
    // py::register_exception<ripple::LogicError>(m, "LogicError");

    
    m
        .def("accountKeylet", &ripple::keylet::account)
        // .def("preflight1", &ripple::preflight1)
        // .def("preflight2", &ripple::preflight2)
        ;
}