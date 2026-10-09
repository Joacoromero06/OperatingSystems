// Translate vaddress to paddress
//
// v address is [VNP | offset]
// we have in r3 the PDBR of the current process
// VPN is [page_directory_index | page_table_index]
//
// We translate the page number to the physical via Page Directory structure
//
#include <stdlib.h>
typedef struct pointer{
  void* p;
  int align;
  size_t size;
} VPointer;

typedef void* PPointer;
PPointer translate(VPointer vaddress) {
  VPN = vaddress.p & MASK_VPN;
  (success, TlbEntry) = TLBlookup(VPN);
  if (success) {
    if (can_access(TlbEntry.prot_bits)) {
      return TlbEntry.PFN << SHIFT_PFN || vaddress.p & MASK_OFFSET;
    } else {
      raise Exception (PROTECTION_FAULT);
    }
  } else {
    PD = access_memory(R3 = PDBR);
    PDE = PD + VPN && MASK_PD_INDEX * sizeof(PDEntry_t);
    if (PDE.valid) {
      PT = access_memory(PDE.PFN);
      PTE = PT + VPN && MASK_PT_INDEX * sizeof(PTEntry_t);
      if (can_access(PTE.prot_bits)) {
        if (PTE.present_bit) { // PAGE HIT
          return PTE.PFN << SHIFT_PFN || vaddress.p & MASK_OFFSET;
        } else { // PAGE FAULT
          PTEreplaced = replacement_policy();
          PFN_new = PTEreplaced.PFN;
          WRITE(PTEreplaced.PFN, free_page_swap_space);
          PTEreplaced.PFN = free_page_swap_space;
          PTErepalced.present_bit = false;

          READ(PFN_new, PTE.PFN=#page in swap space);
          is_free_page_swap_space(PTE.PFN);
          PTE.PFN = PFN_new;
          PTE.present_bit = true;
          translate();
        }
      } else {
        Raise Exception (PROTECTION_FAULT);
      }
    } else {
      raise Exception (SEGMENTATION_FAULT);
    }

  }
}

